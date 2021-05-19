
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
	std::vector<var_ptr> mVariables;
	// std::map<std::shared_ptr<constraint_abstract>, std::vector<var_ptr>> mConstrToVars;
	// std::map<var_ptr, std::vector<std::shared_ptr<constraint_abstract>>> mVarsToConstr;

	size_t mNumEqus;
	int mVerboseLevel;
	int const mMaxIt_LM = 1000, mMaxIt_DL = 1000;
	int mId;

	int mAlgorithm;
public:	
	constraintCluster(std::vector<std::shared_ptr<constraint_abstract>> constraints, std::vector<var_ptr> vars, int solver_algo, int verbose = 0);
	~constraintCluster();	
	
	void init();

	bool satisfied();

	int solver() { return mAlgorithm; }
	void set_solver(int algo) { mAlgorithm = algo; }

	int solve();

	/*
		@function solve_LM is a faithful implementation of the Levenberg-Marquardt algorithm
		it is slower than the naive counter part, but can go to much higher precision
	*/
	int solve_LM(double eps1 = 1e-24, int tag = 0, bool drivingVars_only = false);

	int solve_DL(double eps = 1e-12, int tag = 0, bool drivingVars_only = false);

	void substitutions();

	/*
		@function compute_errors compute the error for each equation in the system

		@param errors [output] : An array of errors of size n_constraints
	*/
	void compute_errors(Eigen::VectorXd& errors, int tag = 0);

	/*
		@function compute_jacobi computes the jacobi of the system

		@param jacobi [output] : The matrix of size (n_constraints x n_variables) containing
		every partial derivative of every equation with respect to every variable
	*/
	void compute_jacobi(Eigen::MatrixXd& jacobi, int tag = 0, bool drivingVars_only = false);

	/*
		@function incr_variables increments the variables values by a delta

		@param delta : The change in the variables values
	*/
	void incr_variables(Eigen::VectorXd const& delta, bool drivingVars_only = false);

	/*
		@function set_variables sets the values of the variables

		@param values : The values to assign to the variables
	*/
	void set_variables(Eigen::VectorXd const& values, bool drivingVars_only = false);

	/*
		@function retrieve_variables assigns the variables values to a given array

		@param container [output] : The array that will be filled with the variables' values
	*/
	void retrieve_variables(Eigen::VectorXd& container, bool drivingVars_only = false);

	void clear_tags();
	void clear_substitutions();

	void set_id(int id_) { mId = id_; }
	int id() const { return mId; }
private:
	void compute_errors_no_resize(Eigen::VectorXd& errors, int tag = 0);
	void compute_jacobi_no_resize(Eigen::MatrixXd& jacobi, int tag = 0, bool drivingVars_only = false);
	int num_taggedEqus(int tag);
	int num_drivingVars();
};

#endif