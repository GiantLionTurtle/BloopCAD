
#ifndef EQUATIONSSYSTEM_HPP_
#define EQUATIONSSYSTEM_HPP_

#include "expression.hpp"

#include <Eigen/Eigen>

#include <vector>

class equationsSystem {
public:
	static constexpr double kEpsilon = 1e-10;
private:
	std::vector<expression_ptr> mEquations;
	std::vector<variable_ptr> mVariables;
    Eigen::MatrixXd mJacobianMat;
	Eigen::VectorXd mErrors;
	Eigen::VectorXd mComputedF;
	bool mComputedF_upToDate;
	int mMaxIt;
public:
	equationsSystem();
	equationsSystem(std::vector<expression_ptr> eqs, std::vector<variable_ptr> vars = {});

	void add_equation(expression_ptr eq);
	void add_equations(std::vector<expression_ptr> eqs);

	void add_variable(variable_ptr var);
	void add_variables(std::vector<variable_ptr> vars);

	size_t size() const { return mVariables.size(); };

	int maxIt() const { return mMaxIt; }
	void set_maxIt(int maxIt_) { mMaxIt = maxIt_; }

	bool satisfied();

	int solve(bool verbose = false);
private:
	int solve_NewtonRaphson(bool verbose = false);
	int solve_LevenbergMarquardt(bool verbose = false);

	void compute_errors();

	void freeze_allVars();
	void compute_jacobian();
	void compute_F();
	void update_variables(Eigen::VectorXd const& y);
};

#endif