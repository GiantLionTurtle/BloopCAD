
#ifndef CONSTRAINTCLUSTER_HPP_
#define CONSTRAINTCLUSTER_HPP_

#include "expression.hpp"
#include "constraint.hpp"

#include <vector>

#include <Eigen/Eigen>

class constraintCluster {
public:
    enum solveOutput { RUNNING, SUCCESS, FAILURE, INVALID };
public:
	std::vector<std::shared_ptr<constraint_abstract>> mConstraints;
	std::vector<variable_ptr> mVariables;
	std::map<std::shared_ptr<constraint_abstract>, std::vector<variable_ptr>> mConstrToVars;
	std::map<variable_ptr, std::vector<std::shared_ptr<constraint_abstract>>> mVarsToConstr;

	int mVerboseLevel;
	int const mMaxIt_LM = 100, mMaxIt_DL = 100;
	int mId;

	int mAlgorithm;
public:	
	constraintCluster(std::vector<std::shared_ptr<constraint_abstract>> constraints, std::vector<variable_ptr> vars, int solver_algo, int verbose = 0);
	~constraintCluster();	
	
	void init();

	bool satisfied();

	int solver() { return mAlgorithm; }
	void set_solver(int algo) { mAlgorithm = algo; }

	int solve();
	/*
		@function solve_LM attempts to solve the system using the Levenberg-Marquardt algorithm

		Note: it tries to solve it naivly to decrease the error quickly, then with a more faithful
		implementation to get to an acceptable error
	*/	
	int solve_LM();

	/*
		@function solve_LM_faithful is a faithful implementation of the Levenberg-Marquardt algorithm
		it is slower than the naive counter part, but can go to much higher precision
	*/
	int solve_LM_faithful(double eps1 = 1e-24);

	/*
		@function solve_LM_naive is a naive implementation of the Levenberg-Marquardt algorithm

		@param eps1 : The success threshold for the square error [defaults to 1e-12]
	*/
	int solve_LM_naive(double eps1 = 1e-12);

	int solve_LM2(double eps = 1e-24);

	int solve_DL(double eps = 1e-12);

	/*
		@function compute_errors compute the error for each equation in the system

		@param errors [output] : An array of errors of size n_constraints
	*/
	void compute_errors(Eigen::VectorXd& errors);

	/*
		@function compute_jacobi computes the jacobi of the system

		@param jacobi [output] : The matrix of size (n_constraints x n_variables) containing
		every partial derivative of every equation with respect to every variable
	*/
	void compute_jacobi(Eigen::MatrixXd& jacobi);

	/*
		@function incr_variables increments the variables values by a delta

		@param delta : The change in the variables values
	*/
	void incr_variables(Eigen::VectorXd const& delta);

	/*
		@function set_variables sets the values of the variables

		@param values : The values to assign to the variables
	*/
	void set_variables(Eigen::VectorXd const& values);

	/*
		@function retrieve_variables assigns the variables values to a given array

		@param container [output] : The array that will be filled with the variables' values
	*/
	void retrieve_variables(Eigen::VectorXd& container);

	void set_id(int id_) { mId = id_; }
	int id() const { return mId; }
};

#endif