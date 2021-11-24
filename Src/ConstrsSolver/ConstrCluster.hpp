
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CONSTRCLUSTER_HPP_
#define CONSTRCLUSTER_HPP_

#include <Utils/Param.hpp>
#include "Constraint.hpp"

#include <Eigen/Eigen>

#include <vector>

class ConstrCluster {
private:
	std::vector<Param*> mParams { }, mParamsSubst { }, mParamsDriving { }, mParamsStandBy;

	std::vector<Constraint*> mConstrsSubst { }, mConstrsEval;

	Eigen::VectorXd mErrors;
	int mAlgorithm { 0 };
	double mMaxError { CONSTR_SATISFACTION_TRESHOLD };
	const int mMaxIt_LM { 50 }, mMaxIt_DL { 50 };
public:
	ConstrCluster() = default;
	ConstrCluster(int algo);

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

	void add_constr(Constraint* constr);
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