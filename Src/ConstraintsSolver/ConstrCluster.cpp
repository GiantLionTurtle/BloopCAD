
#include "ConstrCluster.hpp"
#include "SolverState.hpp"
#include <Utils/Debug_util.hpp>

int ConstrCluster::solve()
{
	substitute();

	int out = solve_numeric();

	clear_drag();

	if(out != SolverState::solveOutput::SUCCESS) {
		substitute();
		out = solve_numeric();
	}
	
	clear_substitutions();
	
	return out;
}

int ConstrCluster::solve_numeric()
{
	switch(mAlgorithm) {
		case SolverState::DogLeg:
			return solve_DL();
			break;
		case SolverState::LevenbergMarquardt:
			return solve_LM();
			break;
		default: // Grrrrr
			LOG_WARNING("Unknown solver " + std::to_string(mAlgorithm));
			return SolverState::FAILURE;
			break;
	}
}

int ConstrCluster::solve_LM()
{
	// Lots of maths
	// Taken from http://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf

	verbose(VERBOSE_STEPS, "LM solver...");
	int n_params = mParamsDriving.size();		// Number of variables in the system
	int n_constrs  = mConstrsEval.size();;	// Number of equations in the system

	if(n_constrs  == 0) {
		verbose(VERBOSE_STEPS, "Returning early, no equation to solve.");
		update_substitution();
		return SolverState::SUCCESS;
	} else if(n_params == 0) {
		verbose(VERBOSE_STEPS, "Returning early, no variable to solve for.");
		update_substitution();
		return SolverState::SUCCESS;
	}

	Eigen::VectorXd P(n_params), dP(n_params), P_new(n_params), 	// Variables values, attempt change in variables and candidate variables values
					e_new(n_constrs ), g(n_params), 	// Error over variables, candidate variables and error scaled with jacobian  
					A_diag(n_params);							// Diagonal of approximation of Hessian matrix
	mErrors.resize(n_constrs);
	Eigen::MatrixXd J(n_constrs , n_params), A(n_params, n_params); 	// Jacobian matrix and approximation of Hessian matrix

	double eps1 = std::sqrt(mMaxError);
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
	int n_params = mParamsDriving.size();		// Number of variables in the system
	int n_constrs  = mConstrsEval.size();	// Number of equations in the system
	verbose(VERBOSE_INNERSTEPS,	"Num tagged: "<<n_constrs );
	
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
	double eps3 = eps;
	
	// Compute and retrieve the initial state of the system
	retrieve_params(P);
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

		if(h_dl.norm() <= eps2 * (P.norm() + eps2)) {
			output = SolverState::SUCCESS;
			break;
		}

		double h_scale = stepScale();
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
			output = (mErrors.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? SolverState::SUCCESS : SolverState::RUNNING;
		}
		if(rho > 0.75) {
			r = std::max(r, 3 * h_dl.norm()); // Update trust region
		} else if(rho < 0.25) {
			r = r / 2.0; // Shrink trust region
			output = (r < eps2 * (P.norm() + eps2)) ? SolverState::SUCCESS : SolverState::RUNNING; 	// If this is true, other stop criterions 
																									// will be true next loop
		}
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
	// return output;
	return satisfied() ? SolverState::solveOutput::SUCCESS : SolverState::solveOutput::FAILURE;
}

double ConstrCluster::stepScale()
{
	double scale = 1.0;
	for(int i = 0; i < mConstrsEval.size(); ++i) {
		scale = std::min(scale, mConstrsEval[i]->stepScale(mErrors(i)));
	}
	return scale;
}

void ConstrCluster::add_constr(Constraint_abstr* constr)
{
	if(constr->substitutionConstraint()) { 
		mConstrsSubst.push_back(constr);
	} else {
		mConstrsEval.push_back(constr);
	}
}
void ConstrCluster::add_param(Param* param)
{
	mParams.push_back(param);
}

void ConstrCluster::clear()
{
	mParams.clear();
	mConstrsSubst.clear();
	mConstrsEval.clear();
}

void ConstrCluster::substitute()
{
	for(auto constr : mConstrsSubst) {
		constr->substitute();
	}
}
void ConstrCluster::clear_substitutions()
{
	for(auto p : mParamsSubst) {
		p->delete_substitution();
	}
}
void ConstrCluster::clear_drag()
{
	for(auto p : mParamsStandBy) {
		p->set_frozen(false);
	}
}
void ConstrCluster::order_params()
{
	mParamsDriving.clear();
	mParamsSubst.clear();
	mParamsStandBy.clear();

	for(int i = 0; i < mParams.size(); ++i) {
		if(mParams[i]->substituted()) {
			mParamsSubst.push_back(mParams[i]);
		} else if(!mParams[i]->frozen()) {
			mParamsDriving.push_back(mParams[i]);
		} else {
			mParamsStandBy.push_back(mParams[i]);
		}
	}
	// There might be a hole in the middle of the array for frozen parameters
	// Anyway those don't matter for the solve
}

void ConstrCluster::update_params(double* vals)
{
	// Don't know if all this unrolling is that necessary, just found out about
	// Duff's device and wanted to try 
	// https://en.wikipedia.org/wiki/Duff's_device

	// Set the values of the valid parameters (not dragged nor substituted parameters)
	{
		int n = (mParamsDriving.size()+7) / 8;
		Param** validP = &mParamsDriving.front();

		switch(mParamsDriving.size() % 8) {
			case 0: (*validP++)->set(*vals++);
			case 7: (*validP++)->set(*vals++);
			case 6: (*validP++)->set(*vals++);
			case 5: (*validP++)->set(*vals++);
			case 4: (*validP++)->set(*vals++);
			case 3: (*validP++)->set(*vals++);
			case 2: (*validP++)->set(*vals++);
			case 1: (*validP++)->set(*vals++);
		}
		while(--n > 0) {
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
		}
	}
	update_substitution();
}

void ConstrCluster::update_substitution()
{
	// Set the substitutions of substituted parameters
	{
		int n = (mParamsSubst.size()+7) / 8;
		Param** substP = &mParamsSubst.front();

		switch(mParamsSubst.size() % 8) {
			case 0: (*substP--)->apply_substitution();
			case 7: (*substP--)->apply_substitution();
			case 6: (*substP--)->apply_substitution();
			case 5: (*substP--)->apply_substitution();
			case 4: (*substP--)->apply_substitution();
			case 3: (*substP--)->apply_substitution();
			case 2: (*substP--)->apply_substitution();
			case 1: (*substP--)->apply_substitution();
		}
		while(--n > 0) {
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
		}
	}
}

bool ConstrCluster::satisfied()
{
	for(auto c : mConstrsEval) {
		if(!c->satisfied())
			return false;
	}
	return true;
}

void ConstrCluster::retrieve_params(Eigen::VectorXd& P)
{
	for(int i = 0; i < mParamsDriving.size(); ++i) {
		P(i) = mParamsDriving[i]->val();
	}
}
void ConstrCluster::compute_jacobi(Eigen::MatrixXd& J)
{
	for(int i = 0; i < mConstrsEval.size(); ++i) {
		for(int j = 0; j < mParamsDriving.size(); ++j) {
			J(i, j) = mConstrsEval[i]->derivative(mParamsDriving[j]);
		}
	}
}
void ConstrCluster::compute_errors(Eigen::VectorXd& e)
{
	for(int i = 0; i < mConstrsEval.size(); ++i) {
		e(i) = mConstrsEval[i]->error();
	}
}