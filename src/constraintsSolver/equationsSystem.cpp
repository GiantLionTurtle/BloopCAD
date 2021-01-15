
#include "equationsSystem.hpp"

#include <utils/errorLogger.hpp>

void subEquationsSystem::set_constant()
{
	for(int i = 0; i < variables.size(); ++i) {
		variables[i]->set_constant();
	}
}
void subEquationsSystem::set_tmpConstant(bool const_)
{
	for(int i = 0; i < variables.size(); ++i) {
		variables[i]->set_tmpConstant(const_);
	}
}

equationsSystem::equationsSystem():
	mMaxIt(50)
{

}
equationsSystem::equationsSystem(std::vector<expression_ptr> eqs, std::vector<variable_ptr> vars):
	mEquations(eqs),
	mVariables(vars),
	mMaxIt(50)
{
	mComputedF_upToDate = false;
}

void equationsSystem::add_equation(expression_ptr eq)
{
	mComputedF_upToDate = false;
	mEquations.push_back(eq);
}
void equationsSystem::add_equations(std::vector<expression_ptr> eqs)
{
	mComputedF_upToDate = false;
	mEquations.insert(mEquations.end(), eqs.begin(), eqs.end());
}

void equationsSystem::add_variable(variable_ptr var)
{
	mComputedF_upToDate = false;
	mVariables.push_back(var);
}
void equationsSystem::add_variables(std::vector<variable_ptr> vars)
{
	mComputedF_upToDate = false;
	mVariables.insert(mVariables.end(), vars.begin(), vars.end());
}

// void equationsSystem::add_subSystem(subEquationsSystem const& subSystem)
// {
// 	add_variables(subSystem.variables);
// 	add_equations(subSystem.equations);
// }

bool equationsSystem::satisfied()
{
	if(!mComputedF_upToDate)
		compute_F();
	for(size_t i = 0; i < mEquations.size(); ++i) {
		if(std::abs(mComputedF(i)) >= kEpsilon)
			return false;
	}
	return true;
}

int equationsSystem::solve(bool verbose)
{
	if(mVariables.empty()) {
		if(verbose)
			LOG_WARNING("No variables in equation system.");
		return 0; // Success! Our 0 variables are all correct
	}

	// if(mVariables.size() > mEquations.size()) {
		return solve_LevenbergMarquardt(verbose);
	// } else if(mVariables.size() == mEquations.size()) {
	// 	return solve_NewtonRaphson();
	// }

	return -1;
}

std::vector<double> equationsSystem::state()
{
	std::vector<double> state_vec(mVariables.size());
	for(int i = 0; i < mVariables.size(); ++i) {
		state_vec[i] = mVariables[i]->val();
	}
	return state_vec;
}
void equationsSystem::set(std::vector<double> st)
{
	if(st.size() != mVariables.size()) {
		LOG_WARNING("Sizes not matching.");
		return;
	}
	for(int i = 0; i < mVariables.size(); ++i) {
		mVariables[i]->set_val(st[i]);
	}
}

int equationsSystem::solve_NewtonRaphson(bool verbose)
{
	Eigen::VectorXd y(mVariables.size());
	mJacobianMat.resize(mEquations.size(), mVariables.size());
	freeze_allVars();

	for(int i = 0; i < mMaxIt; ++i) {
		compute_F();
		if(satisfied()) {
			return i;
		}
		compute_jacobian();

		y = -mJacobianMat.inverse() * mComputedF;

		update_variables(y);
	}

	if(verbose)
		LOG_WARNING("Could not solve this system in: " + std::to_string(mMaxIt) + " iterations.");
	return -1;
}

int equationsSystem::solve_LevenbergMarquardt(bool verbose)
{
	Eigen::VectorXd y(mVariables.size());
	mJacobianMat.resize(mEquations.size(), mVariables.size());
	Eigen::MatrixXd identity = Eigen::MatrixXd::Identity(mVariables.size(), mVariables.size());
	mErrors.resize(mEquations.size());
	freeze_allVars();
	double lambda = 0.1; // TODO: find a way to compute this??

	compute_F();
	compute_errors();
	double previous_error_norm = mErrors.norm();

	for(int i = 0; i < mMaxIt; ++i) {
		if(satisfied()) {
			return i;
		}
		compute_jacobian();
		y = -(mJacobianMat.transpose() * mJacobianMat + lambda * identity).inverse() * (mJacobianMat.transpose() * mErrors);

		update_variables(y);

		compute_F();
		compute_errors();
		if(mErrors.norm() < previous_error_norm) {
			lambda /= 2.0;
		} else {
			lambda *= 2.0;
		}
	}
	if(verbose)
		LOG_WARNING("Could not solve this system in: " + std::to_string(mMaxIt) + " iterations.");
	return -1;
}

void equationsSystem::compute_errors()
{
	for(int i = 0; i < mEquations.size(); ++i) {
		mErrors(i) = mEquations[i]->eval();
	}
}

void equationsSystem::freeze_allVars()
{
	for(int i = 0; i < mVariables.size(); ++i) {
		mVariables[i]->set_fixed(true);
	}
}
void equationsSystem::compute_jacobian()
{
	for(int i = 0; i < mEquations.size(); ++i) { // Kind of confusing because we want a square at all cost, but this is the equation loop
		for(int j = 0; j < mVariables.size(); ++j) {
			mVariables[j]->set_fixed(false);
			mJacobianMat(i, j) = mEquations[i]->derivative()->eval();
			mVariables[j]->set_fixed(true);
		}
	}
}

void equationsSystem::compute_F()
{
	mComputedF.resize(mEquations.size());
	for(int i = 0; i < mEquations.size(); ++i) {
		mComputedF(i) = mEquations[i]->eval();
	}
	mComputedF_upToDate = true;
}

void equationsSystem::update_variables(Eigen::VectorXd const& y)
{
	if(y.size() < mVariables.size()) {
		LOG_WARNING("Y is smaller thant the number of variables to update");
		return;
	}
	for(int i = 0; i < mVariables.size(); ++i) {
		mVariables[i]->set_val(mVariables[i]->val() + y(i));
	}
}

subEquationsSystem operator-(subEquationsSystem const& l, subEquationsSystem const& r)
{
	if(l.equations.size() != r.equations.size()) {
		LOG_WARNING("Sub equations systems do not mesh.");
		return {};
	}
	subEquationsSystem out = { std::vector<variable_ptr>(0), std::vector<expression_ptr>(l.equations.size()) };
	for(int i = 0; i < l.equations.size(); ++i) {
		out.equations[i] = l.equations[i] - r.equations[i];
	}
	out.variables.insert(out.variables.end(), l.variables.begin(), l.variables.end());
	out.variables.insert(out.variables.end(), r.variables.begin(), r.variables.end());

	return out;
}