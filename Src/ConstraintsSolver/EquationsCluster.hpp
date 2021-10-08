
#ifndef EQUATIONSCLUSTER_HPP_
#define EQUATIONSCLUSTER_HPP_

#include "Expression_forward.hpp"

#include <vector>
#include <set>

#include <Eigen/Eigen>

/*
	@class EquationsCluster numericaly solves cluster of equations with respect to a set 
	of variables
*/
class EquationsCluster {
private:
	std::vector<equ_ptr> mEqus; // All equations to solve
	std::set<var_ptr> mVars;	// Variables that must be tweaked, they are an ordered set to ensure that there is no dupplicate 

	int const mMaxIt_LM = 1000, mMaxIt_DL = 1000; // Max iterations of Levenberg-Marquardt and DogLeg algorithms (in practice it rarely exeeds 5)
	int mId;

	int mAlgorithm, mLastOutput;
public:	
	/*
		@constructor EquationsCluster 

		@param equs The equations in the cluster
		@param vars The variables in the cluster
		@param solver_algo The numerical solver to use [None = 0, DogLeg = 1, LevenbergMarquardt = 2]
	*/
	EquationsCluster(std::vector<equ_ptr> equs, std::set<var_ptr> vars, int solver_algo);
	~EquationsCluster();	

	/*
		@function add_equ adds an equation to the cluster

		@param equ Equation to add
	*/
	void add_equ(equ_ptr equ);
	/*
		@function add_var adds a variable to the variable set

		@param var Variable to add
	*/
	void add_var(var_ptr var);

	/*
		@function satisfied 

		@return If every equation of the cluster is satisfied (it evaluates to 0)
	*/
	bool satisfied();

	/*
		@function solver

		@return The algorithm used to numericaly solve
	*/
	int solver() { return mAlgorithm; }
	/*
		@function set_solver sets the algorithm used to solve numericaly

		@param algo The new algorithm to use
	*/
	void set_solver(int algo) { mAlgorithm = algo; }

	/*
		@function solve solves the cluster with a few strategies

		@return The output state [RUNNING, SUCCESS, FAILURE, INVALID]
		@note It also remember the output state for later interrogation
	*/
	int solve();
	/*
		@function output

		@return The last output from a solve
	*/
	int output() { return mLastOutput; }

	/*
		@function solve_numeric solves some of the equations with some of the variables 
		with a given algorithm

		@param algo 			The algorithm used
		@param tag 				The tag of the equations to solve (only the equations with given tag are solved)
		@param activeVars_only 	Wheter to only solve driving vars or not
	*/
	int solve_numeric(int algo, int tag, bool activeVars_only = false);

	/*
		@function solve_LM is an implementation of the Levenberg-Marquardt algorithm

		@param eps1 [defaults to 1e-24] Is the maximum squared error allowed
		@param tag [defaults to 0] 		Is the tag equations must have to be solved
		@param activeVars_only 			Wheter to only solve driving vars or not
		@note There might be implementation errors that are yet to be discovered as this is a complex bit
	*/
	int solve_LM(double eps1 = 1e-24, int tag = 0, bool activeVars_only = false);
	/*
		@function solve_LM is an implementation of the Levenberg-Marquardt algorithm

		@param eps1 [defaults to 1e-12] Is the maximum error allowed
		@param tag [defaults to 0] 		Is the tag equations must have to be solved
		@param activeVars_only 			Wheter to only solve driving vars or not
		@note There might be implementation errors that are yet to be discovered as this is a complex bit
	*/
	int solve_DL(double eps = 1e-12, int tag = 0, bool activeVars_only = false);

	/*
		@function substitutions create substitutes variable that can be simply expressed as a function of another variable
		(curently it can only deal with equations of form x1 = x2)
		
		@note This first step of solve can save significant time in main numerical solve as it allows to solve 
		equations individually, reducing the size of the jacobi matrix for the main solve
	*/
	void substitutions();

	/*
		@function compute_errors computes the error for each equation in the system

		@param errors [out] An array of errors of size n_constraints
		@param tag [defaults to 0] 					The target equations
	*/
	void compute_errors(Eigen::VectorXd& errors, int tag = 0);

	/*
		@function compute_jacobi computes the jacobi of the system

		@param jacobi [out] The matrix of size (n_constraints x n_variables) into which to put
		every partial derivative of every equation with respect to every variable
		@param tag [defaults to 0] 					The target equations
		@param activeVars_only [defaults to false] 	Whether to only uses active variables 
	*/
	void compute_jacobi(Eigen::MatrixXd& jacobi, int tag = 0, bool activeVars_only = false);

	/*
		@function incr_variables increments the variables values by a delta

		@param delta The change in the variables values
		@param activeVars_only [defaults to false] 	Whether to only uses active variables 
	*/
	void incr_variables(Eigen::VectorXd const& delta, bool activeVars_only = false);

	/*
		@function set_variables sets the values of the variables

		@param values The values to assign to the variables
		@param activeVars_only [defaults to false] 	Whether to only uses active variables 
	*/
	void set_variables(Eigen::VectorXd const& values, bool activeVars_only = false);

	/*
		@function retrieve_variables assigns the variables values to a given array

		@param container [out] The array that will be filled with the variables' values
		@param activeVars_only [defaults to false] 	Whether to only uses active variables 
	*/
	void retrieve_variables(Eigen::VectorXd& container, bool activeVars_only = false);

	/*
		@function clear resets the cluster to a default state
	*/
	void clear();
	/*
		@function clear_tags set all the equations' tags to 0, they are no longer 
		differentiated
	*/
	void clear_tags();
	/*
		@function clear_substitutions removes substitution of each variable

		@note It is often called after @function apply_substitutions
	*/
	void clear_substitutions();
	/*
		@function apply_substitutions sets the value of all variable to their substituant's value
		if they have one.

		@note It is used after solve to make variables independant from their substituant,
		it could be more complex and allow for better behavior if substituant is changed after
		solve (TODO: check if it would work to call it before solve instead of after)
	*/
	void apply_substitutions();

	/*
		@function set_id sets the debug id of this cluster
		
		@param id_ The new debug id
	*/
	void set_id(int id_) { mId = id_; }
	/*
		@function id 

		@return The debug id of this cluster
	*/
	int id() const { return mId; }
private:
	/*
		@function compute_errors_no_resize computes errors but assumes that the given output vector
		has the right dimensions.

		@param errors [out] 		The vector into which to put the errors
		@param tag [defaults to 0] 	The target equations
		@note It should be profiled to verify if it is really helpful, it might skip one (1) jump, 
		but how useful is that in a numerically heavy function...
	*/
	void compute_errors_no_resize(Eigen::VectorXd& errors, int tag = 0);
	/*
		@function compute_jacobi_no_resize computes jacobi but assumes that the given output matrix
		has the right dimensions.

		@param jacobi [out] 						The matrix into which to put the partial derivatives
		@param tag [defaults to 0] 					The target equations
		@param activeVars_only [defaults to false] 	Whether to only uses active variables 
		@note It should be profiled to verify if it is really helpful, it might skip one (1) jump, 
		but how useful is that in a numerically heavy function...
	*/
	void compute_jacobi_no_resize(Eigen::MatrixXd& jacobi, int tag = 0, bool activeVars_only = false);
	/*
		@function n_taggedEqus

		@param tag The target tag of the function
		@return The number of equations with the given tag
	*/
	int n_taggedEqus(int tag);
	/*
		@function n_activeVars
		
		@return The number of active variables (driving)
		@used To create the jacobi matrix if active var is required

	*/
	int n_activeVars();
	/*
		@function active is a utility to check if a variable is active

		@param var Variable to evaluate
		@return If the variable is active
	*/
	static bool active(var_ptr var);
};

#endif