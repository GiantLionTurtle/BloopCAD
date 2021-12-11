
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#include "ConstrCluster.hpp"
#include "SolverState.hpp"

#include <Utils/Debug_util.hpp>

ConstrCluster::ConstrCluster(int algo)
	: mAlgorithm(algo)
{

}

int ConstrCluster::solve_numeric()
{
	switch(mAlgorithm) {
		case SolverState::DogLeg:
			mOutput = solve_DL();
			break;
		case SolverState::LevenbergMarquardt:
			mOutput = solve_LM();
			break;
		default: // Grrrrr
			LOG_WARNING("Unknown solver " + std::to_string(mAlgorithm));
			mOutput = SolverState::FAILURE;
			break;
	}
	return mOutput;
}

int ConstrCluster::solve_LM()
{
	// Lots of maths
	// Taken from http://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf

	verbose(VERBOSE_STEPS, "LM solver...");
	int n_params = mParams.size() - n_dragged();		// Number of variables in the system
	int n_constrs  = mConstrs.size();;	// Number of equations in the system

	if(n_constrs  == 0) {
		verbose(VERBOSE_STEPS, "Returning early, no equation to solve.");
		return SolverState::SUCCESS;
	} else if(n_params == 0) {
		verbose(VERBOSE_STEPS, "Returning early, no variable to solve for.");
		return SolverState::SUCCESS;
	}

	Eigen::VectorXd P(n_params), dP(n_params), P_new(n_params), 	// Variables values, attempt change in variables and candidate variables values
					e_new(n_constrs ), g(n_params), 	// Error over variables, candidate variables and error scaled with jacobian  
					A_diag(n_params);							// Diagonal of approximation of Hessian matrix
	mErrors.resize(n_constrs);
	Eigen::MatrixXd J(n_constrs , n_params), A(n_params, n_params); 	// Jacobian matrix and approximation of Hessian matrix

	double eps1 = mMaxError*mMaxError; // Squared because it uses the error square to compare
	double eps2 = 1e-24; 		// Is part of a change threshold that determines if the solver is stuck
	double tau = 1e-3;			// Serves as a basis for the meta parameter mu
	double mu, nu = 2.0; 		// Meta parameters defining the aggressivity of the steps
	double e_norm, e_new_norm;	// Squared norm of the error on the variables and the candidate variables
	double g_norm;				// Infinity norm of the profuct J.inv * e
	
	// Compute and retrieve the initial state of the system
	retrieve_params(P);
	compute_jacobi(J);
	compute_errors(mErrors);

	mErrors = -mErrors;
	e_norm = mErrors.squaredNorm();
	A = J.transpose() * J;
	g = J.transpose() * mErrors;
	A_diag = A.diagonal();
	mu = tau * A_diag.lpNorm<Eigen::Infinity>();
	
	int output = g.lpNorm<Eigen::Infinity>() <= eps1 ? SolverState::SUCCESS : SolverState::RUNNING; // Check if the error is already low enough
	int k;
	for(k = 0; k < mMaxIt_LM && !output; ++k) {
		if(e_norm <= eps1) { // Error is low enough to exit
			output = SolverState::SUCCESS;
			break;
		}
		if(e_norm != e_norm) { // NaN, exit
			output = SolverState::FAILURE;
			break;
		}

		// Attempt to find a rho that is > 0, 50 iterations max (arbitrary)
		double rho = 0.0;
		int n;
		for(n = 0; n < 50; ++n) {
			for(int i = 0; i < n_params; ++i) { // Augment Hessian approx's diagonal
				A(i, i) += mu;
			}

			// Solve linear system dP*(A + mu*I)=g
			// There are many options, fullPivLu is more precise and requires no particularity of the 
			// matrices, it is however slower. All the choices displayed here *seem* to work
			// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
			dP = A.fullPivLu().solve(g);
			// dP = A.householderQr().solve(g);
			// dP = A.completeOrthogonalDecomposition().solve(g);
			// dP = A.ldlt().solve(g);
			// dP = A.llt().solve(g);
			
			if(((A*dP-g).norm() / g.norm()) <= 1e-5) { // Check if linear system solve was successful
				
				double dp_norm = dP.norm();
				if(dp_norm <= eps2 * P.norm()) { // norm() is L2 norm, change in variables is very small, the solver is likely stuck
					output = SolverState::FAILURE;
					break;
				}

				// Assign new values to the candidate values and compute error
				P_new = P + dP;
				update_params(P_new.data());
				double p_scale = stepScale();
				if(p_scale < 1.0) {
					P_new = P + dP * p_scale;
					update_params(P_new.data());
				}

				compute_errors(e_new);
				e_new = -e_new;
				e_new_norm = e_new.squaredNorm();

				rho = (e_norm - e_new_norm) / (dP.transpose() * (mu * dP + g)); // Compute rho (must be > 0 to escape the inner loop)
				if(rho > 0.0) {
					// Compute & retrieve new state
					P = P_new;
					compute_jacobi(J);
					A = J.transpose() * J;
					A_diag = A.diagonal();
					mErrors = e_new;
					e_norm = e_new_norm;
					g = J.transpose() * mErrors;

					// Check if the error is low enough
					output = g.lpNorm<Eigen::Infinity>() <= eps1 ? SolverState::SUCCESS : SolverState::RUNNING;
					
					// Update meta parameters, mu will get smaller
					double tmp = 2*rho - 1;
					mu *= std::max(1.0/3.0, 1.0 - (tmp*tmp*tmp));
					nu = 2.0;
					break;
				} 
			}
			{
				for(int i = 0; i < n_params; ++i) { // A good rho was not found, update the meta parameters, mu will get bigger
					A(i, i) = A_diag(i); // Reset diagonal
				}
				// update meta parameters
				mu *= nu;
				nu *= 2.0;
			}
		}
	}

	verbose(VERBOSE_STEPS, "Satisfied: "<<std::boolalpha<<satisfied())
	switch(output) {
	case SolverState::SUCCESS:
		verbose(VERBOSE_STEPS, "Succeeded in "<<k<<" iterations ; squared error of "<<e_norm);
		break;
	//case solveOutput::SolverState::FAILURE:
	default:
		verbose(VERBOSE_STEPS, "Failed in "<<k<<" iterations ; squared error of "<<e_norm);
		break;
	}
	// return output;
	return satisfied() ? SolverState::solveOutput::SUCCESS : SolverState::solveOutput::FAILURE;
}

int ConstrCluster::solve_DL()
{
	// That's a lot of math, it was taken from
	// http://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf

	// DogLeg combines Stepest Descent and Gauss Newton with a trust region (or trust radius)

	verbose(VERBOSE_STEPS, "DL solver...");
	int n_params = mParams.size() - n_dragged();		// Number of variables in the system
	int n_constrs = mConstrs.size() + 1;	// Number of equations in the system
	
	verbose(VERBOSE_INNERSTEPS, "Num equations: "<<n_constrs);
	verbose(VERBOSE_INNERSTEPS, "Num params: "<<n_params);

	if(n_constrs  == 0) {
		verbose(VERBOSE_STEPS, "Returning early, no equation to solve.");
		return SolverState::SUCCESS;
	} else if(n_params == 0) {
		verbose(VERBOSE_STEPS, "Returning early, no variable to solve for.");
		return SolverState::SUCCESS;
	}
	Eigen::VectorXd P(n_params), P_new(n_params), // Variables values, candidate variables values
					h_sd(n_params), // Stepest Descent *direction* (not step!)
					h_gn(n_params), // Gauss-Newton step
					h_dl(n_params), 	
					e_new(n_constrs ), g(n_params); 	// Error over variables, candidate variables and error scaled with jacobian  
	mErrors.resize(n_constrs);
	Eigen::MatrixXd J(n_constrs , n_params); 	// Jacobian matrix and approximation of Hessian matrix

	double r = 1.0; // Trust radius (arbitrary at the moment)
	double eps = mMaxError;
	double eps1 = eps;
	double eps2 = eps;
	double eps3 = 10e-20;

	
	// Compute and retrieve the initial state of the system
	retrieve_params(P);
	mOptim_constr.set_init();
	compute_jacobi(J);
	compute_errors(mErrors);

	g = J.transpose() * mErrors; // Initial g
	
	// Check if error is already low enough
	int output = (mErrors.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? SolverState::SUCCESS : SolverState::RUNNING;
	int k;
	for(k = 0; k < mMaxIt_DL && !output; ++k) {

		double alpha = g.norm() / (J * g).norm(); // How far along h_sd is the step in stepest descent
		h_sd = -alpha * g;

		h_gn = J.fullPivLu().solve(-mErrors); // Solve J(x) * h_gn = -f(x)

		if(h_gn.norm() <= r) { // Gauss-Newton step within trust region
			h_dl = h_gn;
		} else if((alpha * h_sd).norm() >= r) { // If the stepest descent step lands outside the trust region
			h_dl = (r / h_sd.norm()) * h_sd; // Farther possible in the stepest descent direction, at the limit of the trust region
		} else {
			// Here h_dl tries to mix both of h_sd and h_gn directions while staying within the trust region
			Eigen::VectorXd a = alpha * h_sd, b = h_gn;
			double c = a.transpose() * (b - a);
			double beta; // Find beta so that ||h_dl|| = r

			// solve quadratic equation to find beta
			if(c <= 0) {
				beta = (-c + std::sqrt(c*c + (b-a).squaredNorm() * (r*r - a.squaredNorm()))) / (b-a).squaredNorm();
			} else {
				beta = (r*r - a.squaredNorm()) / (c + std::sqrt(c*c + (b-a).squaredNorm() * (r*r - a.squaredNorm())));
			}
			h_dl = a + beta * (b - a); // Mix both steps
		}

		P_new = P + h_dl; // Try a new X in a direction in n dimensions
		update_params(P_new.data());
		double p_scale = stepScale();
		if(p_scale < 1.0) {
			P_new = P + h_dl * p_scale;
			update_params(P_new.data());
		}
		compute_errors(e_new);

		double rho = 	(mErrors.squaredNorm() / 2.0 - e_new.squaredNorm() / 2.0) / ((mErrors.squaredNorm() / 2.0) - 
						((mErrors + J * h_dl).squaredNorm() / 2.0));
		if(rho > 0) {
			P = P_new;
			mErrors = e_new;
			compute_jacobi(J);
			g = J.transpose() * mErrors;
		}
		if(rho > 0.75) {
			r = std::max(r, 3 * h_dl.norm()); // Update trust region
		} else if(rho < 0.25) {
			r = r / 2.0; // Shrink trust region
		}
		output = satisfied() ? SolverState::SUCCESS : SolverState::RUNNING;
	}

	verbose(VERBOSE_STEPS, "Satisfied: "<<std::boolalpha<<satisfied())
	switch(output) {
	case SolverState::SUCCESS:
		verbose(VERBOSE_STEPS, "Succeeded in "<<k<<" steps ; squared error of "<<mErrors.squaredNorm());
		break;
	case SolverState::FAILURE:
	default:
		verbose(VERBOSE_STEPS, "Failed in "<<k<<" steps ; squared error of "<<mErrors.squaredNorm()<<" ; Gaming Gamer UwU");
		break;
	}
	return output == SolverState::SUCCESS ? SolverState::SUCCESS : SolverState::FAILURE;
}

double ConstrCluster::stepScale()
{
	double scale = 1.0;
	for(int i = 0; i < mConstrs.size(); ++i) {
		scale = std::min(scale, mConstrs[i]->stepScale(mErrors(i)));
	}
	std::cout<<"Step scale: "<<scale<<"\n";
	return scale;
}

void ConstrCluster::add_constr(Constraint* constr)
{
	mConstrs.push_back(constr);
}
void ConstrCluster::add_param(Param* param)
{
	mParams.push_back(param);
}

void ConstrCluster::clear()
{
	mParams.clear();
	mConstrs.clear();
}

void ConstrCluster::update_params(double* vals)
{
	for(auto p : mParams) {
		if(p->frozen() > Param::Frozen_levels::UNFROZEN)
			continue;
		p->set(*vals++);
	}
}

int ConstrCluster::n_dragged()
{
	int count = 0;
	for(auto p : mParams) {
		if(p->frozen() > Param::Frozen_levels::UNFROZEN)
			count++;
	}
	return count;
}
bool ConstrCluster::satisfied()
{
	for(auto c : mConstrs) {
		if(!c->satisfied())
			return false;
	}
	return true;
}

void ConstrCluster::retrieve_params(Eigen::VectorXd& P_out)
{
	int i = 0;
	for(auto p : mParams) {
		if(p->frozen() > Param::Frozen_levels::UNFROZEN)
			continue;
		P_out(i++) = p->val();
	}
}
void ConstrCluster::compute_jacobi(Eigen::MatrixXd& J)
{
	int j = 0;
	for(int i = 0; i < mConstrs.size(); ++i) {
		for(auto p : mParams) {
			if(p->frozen() > Param::Frozen_levels::UNFROZEN)
				continue;
			J(i, j++) = mConstrs[i]->derivative(p);
		}
	}

	j = 0;
	for(auto p : mParams) {
		if(p->frozen() > Param::Frozen_levels::UNFROZEN)
			continue;
		J(mConstrs.size(), j++) = mOptim_constr.derivative(p);
	}
}
void ConstrCluster::compute_errors(Eigen::VectorXd& e)
{
	for(int i = 0; i < mConstrs.size(); ++i) {
		e(i) = mConstrs[i]->error();
	}

	e(mConstrs.size()) = mOptim_constr.error();
}