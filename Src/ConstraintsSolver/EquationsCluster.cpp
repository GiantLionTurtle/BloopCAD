
#include "EquationsCluster.hpp"
#include "Expression.hpp"
#include "SolverState.hpp"
#include <Utils/Debug_util.hpp>

#include <iostream>

EquationsCluster::EquationsCluster(std::vector<equ_ptr> equs, std::set<var_ptr> vars, int solver_algo):
	mEqus(equs),
	mVars(vars),
	mId(0),
	mAlgorithm(solver_algo),
	mLastOutput(SolverState::solveOutput::INVALID)
{

}
EquationsCluster::~EquationsCluster()
{

}

void EquationsCluster::add_equ(equ_ptr equ)
{
	if(!equ) { // Wait that's illegal
		LOG_WARNING("Attempted to add null equation");
		return;
	}
	mEqus.push_back(equ);
}
void EquationsCluster::add_var(var_ptr var)
{
	if(!var) { // You can't do that >:(
		LOG_WARNING("Attempted to add null equation");
		return;
	}
	mVars.insert(var);
}

bool EquationsCluster::satisfied()
{
	// TOOD make this 1e-12 less arbitrary... it is maintened by 
	// multiple sources throughout (sometimes squared to 1e-24)
	for(auto equ : mEqus) {
		if(std::abs(equ->eval()) > 1e-12) // TODO: remove magic number and make it more formal
			return false;
	}
	return true;
}

int EquationsCluster::solve()
{
	mLastOutput = SolverState::FAILURE;

	clear_substitutions(); // Fresh start for substitutions
	clear_tags(); 
	substitutions(); // Create substitutions

	int single_tag = 1; // > 0, tag=0 are for equations not solved individually
	for(auto equ : mEqus) {
		auto var = equ->get_single_var(); // Fetch a substituted/lonely variable from equation
		if(var) {
			equ->set_tag(single_tag); // Now this is the only equation with this tag in the cluster, it will be solved alone
			if(equ->eval() <= 1e-12)
				continue;
			mLastOutput = solve_numeric(mAlgorithm, single_tag, true); // Solve with tag

			if(mLastOutput != SolverState::SUCCESS) { // If it fails here, no need to continue, it might be useful in the future to try to recover tho
				verbose(VERBOSE_INNERSTEPS, "Failed early at single tag "<<single_tag);
				mLastOutput = SolverState::FAILURE;
				return mLastOutput;
			}
			single_tag++; // Create new unique tag
		}
	}
	// apply_substitutions(); // Seemed to increase numerical stability (TODO: check)
	verbose(VERBOSE_INNERSTEPS, "Solved "<<single_tag-1<<(single_tag < 2 ? "equation" : "equations")<<" alone");
	
	mLastOutput = solve_numeric(mAlgorithm, 0, true); // Solve the rest of the equations
	apply_substitutions();  // This needs to be here to not lose solve result on substituted variables, 
							// TODO: check if previous apply_substitutions is also needed
	return mLastOutput;
}

int EquationsCluster::solve_numeric(int algo, int tag, bool activeVars_only)
{
	switch(mAlgorithm) {
		case SolverState::DogLeg:
			return solve_DL(1e-14, tag, activeVars_only); // Dogleg can have a lower error (two orders of magnitude)
			break;
		case SolverState::LevenbergMarquardt:
			return solve_LM(1e-24, tag, activeVars_only); // Remember error is squared (e = 1e-12)
			break;
		default: // Grrrrr
			LOG_WARNING("Unknown solver " + std::to_string(mAlgorithm));
			return SolverState::FAILURE;
			break;
	}
}

void EquationsCluster::substitutions()
{
	for(auto equ : mEqus) {
		var_ptr a, b;
		if(equ->can_substitute()) {
			equ->get_substitution_vars(a, b); // Get both variables of the equations (there can only be two for the moment)

			if(mVars.find(a) == mVars.end() || mVars.find(b) == mVars.end()) // Check if variable is in cluster (TODO: really useful???)
				continue;

			// Check which variable is gonna drive, TODO: check edge case where both variables are dragged??
			if(a->dragged()) {
				b->substitute(a);
			} else {
				a->substitute(b);
			}
		}
	}
}

void EquationsCluster::compute_errors(Eigen::VectorXd& errors, int tag)
{
	errors.resize(n_taggedEqus(tag));
	compute_errors_no_resize(errors, tag);
}
void EquationsCluster::compute_errors_no_resize(Eigen::VectorXd& errors, int tag)
{
	int e = 0;
	for(auto equ : mEqus) { // Range useful because e doesn't increment at each loop
		if(equ->tag() == tag)
			errors(e++) = equ->eval(); // Error is just evaluation of the equation (which is a substraction)
	}
}
void EquationsCluster::compute_jacobi(Eigen::MatrixXd& jacobi, int tag, bool activeVars_only)
{
	jacobi.resize(n_taggedEqus(tag), activeVars_only ? n_activeVars() : mVars.size());
	compute_jacobi_no_resize(jacobi, tag, activeVars_only);
}
void EquationsCluster::compute_jacobi_no_resize(Eigen::MatrixXd& jacobi, int tag, bool activeVars_only)
{
	int e = 0, v = 0;
	for(auto equ : mEqus) { // Range useful because e does not increment at each loop (sometimes skipped by 'continue')
		if(equ->tag() != tag) 
			continue;
		for(auto var : mVars) { // Similar to e for range
			if(!activeVars_only || active(var)) {
				double val = equ->derivative_eval(var); // Gets partial derivative with respect to variable
				jacobi(e, v++) = val;
			}
		}
		e++;
		v = 0;
	}
}
int EquationsCluster::n_taggedEqus(int tag)
{
	int n_equs_tagged = 0;
	for(auto equ : mEqus) {
		if(equ->tag() == tag)
			n_equs_tagged++;
	}
	return n_equs_tagged;
}
int EquationsCluster::n_activeVars()
{
	int n_vars_active = 0;
	for(auto var : mVars) {
		if(active(var))
			n_vars_active++;
	}
	return n_vars_active;
}
bool EquationsCluster::active(var_ptr var)
{
	return !var->dragged() && !var->is_substituted();
}

void EquationsCluster::incr_variables(Eigen::VectorXd const& delta, bool activeVars_only) 
{
	int v = 0;
	for(auto var : mVars) {
		if(!activeVars_only || active(var))
			var->set(var->eval() + delta(v++));
	}
}

void EquationsCluster::set_variables(Eigen::VectorXd const& values, bool activeVars_only)
{
	int v = 0;
	for(auto var : mVars) {
		if(!activeVars_only || active(var))
			var->set(values(v++));
	}
}

void EquationsCluster::retrieve_variables(Eigen::VectorXd& container, bool activeVars_only) 
{
	int v = 0;
	for(auto var : mVars) {
		if(!activeVars_only || active(var))
			container(v++) = var->eval();
	}
}

void EquationsCluster::clear()
{
	// Reset everything
	mEqus.clear();
	mVars.clear();
	mId = 0;
	mLastOutput = SolverState::solveOutput::INVALID;
	mAlgorithm = SolverState::algorithm::None;
}
void EquationsCluster::clear_tags()
{
	for(auto equ : mEqus) {
		equ->set_tag(0);
	}
}
void EquationsCluster::clear_substitutions()
{
	for(auto var : mVars) {
		var->clear_substitution();
	}
}
void EquationsCluster::apply_substitutions()
{
	for(auto var : mVars) {
		var->apply_substitution();
	}
}

int EquationsCluster::solve_LM(double eps1, int tag, bool activeVars_only)
{
	// Lots of maths
	// Taken from http://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf

	verbose(VERBOSE_STEPS, "LM solver...");
	int n_vars = activeVars_only ? n_activeVars() : mVars.size();		// Number of variables in the system
	int n_equs = n_taggedEqus(tag);	// Number of equations in the system
	verbose(VERBOSE_INNERSTEPS, "Num tagged: "<<n_equs);
	if(n_equs == 0)
		return SolverState::SUCCESS;
	Eigen::VectorXd P(n_vars), dP(n_vars), P_new(n_vars), 	// Variables values, attempt change in variables and candidate variables values
					e(n_equs), e_new(n_equs), g(n_vars), 	// Error over variables, candidate variables and error scaled with jacobian  
					A_diag(n_vars);							// Diagonal of approximation of Hessian matrix
	Eigen::MatrixXd J(n_equs, n_vars), A(n_vars, n_vars); 	// Jacobian matrix and approximation of Hessian matrix

	double eps2 = 1e-24; 		// Is part of a change threshold that determines if the solver is stuck
	double tau = 1e-3;			// Serves as a basis for the meta parameter mu
	double mu, nu = 2.0; 		// Meta parameters defining the aggressivity of the steps
	double e_norm, e_new_norm;	// Squared norm of the error on the variables and the candidate variables
	double g_norm;				// Infinity norm of the profuct J.inv * e
	
	// Compute and retrieve the initial state of the system
	retrieve_variables(P, activeVars_only);
	compute_jacobi_no_resize(J, tag, activeVars_only);
	compute_errors_no_resize(e, tag);
	e = -e;
	e_norm = e.squaredNorm();
	A = J.transpose() * J;
	g = J.transpose() * e;
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
			for(int i = 0; i < n_vars; ++i) { // Augment Hessian approx's diagonal
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
			
			if((A*dP-g).norm() / g.norm() <= 1e-5) { // Check if linear system solve was successful
				double dp_norm = dP.norm();
				if(dp_norm <= eps2 * P.norm()) { // norm() is L2 norm, change in variables is very small, the solver is likely stuck
					output = SolverState::FAILURE;
					break;
				}


				// Assign new values to the candidate values and compute error
				P_new = P + dP;
				set_variables(P_new, activeVars_only);
				compute_errors_no_resize(e_new, tag);
				e_new = -e_new;
				e_new_norm = e_new.squaredNorm();

				rho = (e_norm - e_new_norm) / (dP.transpose() * (mu * dP + g)); // Compute rho (must be > 0 to escape the inner loop)
				if(rho > 0.0) {
					// Compute & retrieve new state
					P = P_new;
					compute_jacobi_no_resize(J, tag, activeVars_only);
					A = J.transpose() * J;
					A_diag = A.diagonal();
					e = e_new;
					e_norm = e_new_norm;
					g = J.transpose() * e;

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
				for(int i = 0; i < n_vars; ++i) { // A good rho was not found, update the meta parameters, mu will get bigger
					A(i, i) = A_diag(i); // Reset diagonal
				}
				// update meta parameters
				mu *= nu;
				nu *= 2.0;
			}
		}
	}

	set_variables(P, activeVars_only);
	switch(output) {
	case SolverState::SUCCESS:
		verbose(VERBOSE_STEPS, "Succeeded in "<<k<<" iterations ; squared error of "<<e_norm);
		break;
	//case solveOutput::SolverState::FAILURE:
	default:
		verbose(VERBOSE_STEPS, "Failed in "<<k<<" iterations ; squared error of "<<e_norm);
		break;
	}
	return output;
}

int EquationsCluster::solve_DL(double eps, int tag, bool activeVars_only)
{
	// That's a lot of math, it was taken from
	// http://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf

	// DogLeg combines Stepest Descent and Gauss Newton with a trust region (or trust radius)

	verbose(VERBOSE_STEPS, "DL solver...");
	int n_vars = activeVars_only ? n_activeVars() : mVars.size();		// Number of variables in the system
	int n_equs = n_taggedEqus(tag);	// Number of equations in the system
	verbose(VERBOSE_INNERSTEPS,	"Num tagged: "<<n_equs);
	
	if(n_equs == 0)
		return SolverState::SUCCESS;
	Eigen::VectorXd X(n_vars), X_new(n_vars), X_init(n_vars), // Variables values, candidate variables values and initial valies
					h_sd(n_vars), // Stepest Descent *direction* (not step!)
					h_gn(n_vars), // Gauss-Newton step
					h_dl(n_vars), 	
					e(n_equs), e_new(n_equs), g(n_vars); 	// Error over variables, candidate variables and error scaled with jacobian  
	Eigen::MatrixXd J(n_equs, n_vars); 	// Jacobian matrix and approximation of Hessian matrix

	double r = 1.0; // Trust radius (arbitrary at the moment)
	double eps1 = eps;
	double eps2 = eps;
	double eps3 = eps;
	
	// Compute and retrieve the initial state of the system
	retrieve_variables(X, activeVars_only);
	X_init = X;
	compute_jacobi_no_resize(J, tag, activeVars_only);
	compute_errors_no_resize(e, tag);

	g = J.transpose() * e; // Initial g
	
	// Check if error is already low enough
	int output = (e.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? SolverState::SUCCESS : SolverState::RUNNING;
	int k;
	for(k = 0; k < mMaxIt_DL && !output; ++k) {

		double alpha = g.norm() / (J * g).norm(); // How far along h_sd is the step in stepest descent
		h_sd = -alpha * g;

		h_gn = J.fullPivLu().solve(-e); // Solve J(x) * h_gn = -f(x)

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

		if(h_dl.norm() <= eps2 * (X.norm() + eps2)) {
			output = SolverState::SUCCESS;
			break;
		}

		X_new = X + h_dl; // Try a new X in a direction in n dimensions
		set_variables(X_new, activeVars_only); // Update variables (must do it in loop to compute errors)
		compute_errors_no_resize(e_new, tag);

		double rho = (e.squaredNorm() / 2.0 - e_new.squaredNorm() / 2.0) / ((e.squaredNorm() / 2.0) - ((e + J * h_dl).squaredNorm() / 2.0));
		if(rho > 0) {
			X = X_new;
			e = e_new;
			compute_jacobi_no_resize(J, tag, activeVars_only);
			g = J.transpose() * e;
			output = (e.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? SolverState::SUCCESS : SolverState::RUNNING;
		}
		if(rho > 0.75) {
			r = std::max(r, 3 * h_dl.norm()); // Update trust region
		} else if(rho < 0.25) {
			r = r / 2.0; // Shrink trust region
			output = (r < eps2 * (X.norm() + eps2)) ? SolverState::SUCCESS : SolverState::RUNNING; 	// If this is true, other stop criterions 
																									// will be true next loop
		}
	}

	if(output != SolverState::SUCCESS)
		set_variables(X_init, activeVars_only);

	switch(output) {
	case SolverState::SUCCESS:
		verbose(VERBOSE_STEPS, "Succeeded in "<<k<<" steps ; squared error of "<<e.squaredNorm());
		break;
	case SolverState::FAILURE:
	default:
		verbose(VERBOSE_STEPS, "Failed in "<<k<<" steps ; squared error of "<<e.squaredNorm()<<" ; Gaming Gamer UwU");
		break;
	}

	return output;
}