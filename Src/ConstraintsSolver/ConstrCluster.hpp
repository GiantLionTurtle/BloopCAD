
#ifndef CONSTRCLUSTER_HPP_
#define CONSTRCLUSTER_HPP_

#include <Utils/Param.hpp>
#include "Constraint_abstr.hpp"

#include <Eigen>

#include <vector>

class ConstrCluster {
private:
	std::vector<Param*> mParams { }, mParamsSubst { }, mParamsDriving { }, mParamsStandBy;

	std::vector<Constraint_abstr*> mConstrsSubst { }, mConstrsEval;

	Eigen::VectorXd mErrors;
	int mAlgorithm;
	double mMaxError;
	const int mMaxIt_LM { 50 }, mMaxIt_DL { 50 };
public:
	ConstrCluster() = default;

	inline int algorithm() { return mAlgorithm; }
	inline void set_algorithm(int algo) { mAlgorithm = algo; }

	inline double maxError() { return mMaxError; }
	inline void set_maxError(double err) { mMaxError = err; }

	int solve();

	/*
		@function solve_numeric solves some of the equations with some of the variables 
		with the current algorithm
	*/
	int solve_numeric();

	/*
		@function solve_LM is an implementation of the Levenberg-Marquardt algorithm

		@note There might be implementation errors that are yet to be discovered as this is a complex bit
	*/
	int solve_LM();
	/*
		@function solve_LM is an implementation of the Levenberg-Marquardt algorithm
		@note There might be implementation errors that are yet to be discovered as this is a complex bit
	*/
	int solve_DL();

	double stepScale();

	void add_constr(Constraint_abstr* constr);
	void add_param(Param* param);
	void clear();

	void substitute();
	void clear_substitutions();
	void clear_drag();

	void order_params();
	void order_constrs();
	void update_params(double* vals);
	void update_substitution();

	bool satisfied();

	void retrieve_params(Eigen::VectorXd& P);
	void compute_jacobi(Eigen::MatrixXd& J);
	void compute_errors(Eigen::VectorXd& e);
};

#endif