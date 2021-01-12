
#include "equationsSystem.hpp"

#include <utils/errorLogger.hpp>

equationsSystem::equationsSystem():
	mMaxIt(20)
{

}
equationsSystem::equationsSystem(std::vector<expression_ptr> eqs, std::vector<variable_ptr> vars):
	mEquations(eqs),
	mVariables(vars),
	mMaxIt(20)
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

int equationsSystem::solve()
{
	if(mVariables.empty()) {
		LOG_WARNING("No variables in equation system.");
		return -1;
	}

	if(mVariables.size() > mEquations.size()) {
		return solve_LevenbergMarquardt();
	} else if(mVariables.size() == mEquations.size()) {
		return solve_NewtonRaphson();
	}

	return -1;
}

int equationsSystem::solve_NewtonRaphson()
{
	Eigen::VectorXd y(mVariables.size());
	mJacobianMat.resize(mEquations.size(), mVariables.size());

	for(int i = 0; i < mMaxIt; ++i) {
		compute_F();
		if(satisfied()) {
			return i;
		}
		freeze_allVars();
		compute_jacobian();

		y = -mJacobianMat.inverse() * mComputedF;

		update_variables(y);
	}

	LOG_WARNING("Could not solve this system in: " + std::to_string(mMaxIt) + " iterations.");
	return -1;
}

int equationsSystem::solve_LevenbergMarquardt()
{
	// mJacobianMat.resize(mEquations.size(), mVariables.size());
	// double u = 0.5;
	// double u_c;

	// for(int i = 0; i < mMaxIt; ++i) {
	// 	compute_F();
	// 	if(satisfied()) {
	// 		return i;
	// 	}
	// 	freeze_allVars();
	// 	compute_jacobian();
	// 	u_c = u * mComputedF.squaredNorm();
	// 	// y = -mJacobianMat.inverse() * mComputedF;

	// 	update_variables(y);
	// }

	LOG_WARNING("Could not solve this system in: " + std::to_string(mMaxIt) + " iterations.");
	return -1;
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