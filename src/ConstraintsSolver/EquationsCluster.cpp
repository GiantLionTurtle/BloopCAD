
#include "EquationsCluster.hpp"
#include "SolverState.hpp"
#include <utils/DebugUtils.hpp>

#include <iostream>

EquationsCluster::EquationsCluster(std::vector<equ_ptr> equs, std::set<var_ptr> vars, int solver_algo, int verbose):
	mEqus(equs),
	mVars(vars),
	mVerboseLevel(verbose),
	mId(0),
	mAlgorithm(solver_algo)
{

}
EquationsCluster::~EquationsCluster()
{

}

void EquationsCluster::init()
{

}

void EquationsCluster::add_equ(equ_ptr equ)
{
	if(!equ) {
		LOG_WARNING("Attempted to add null equation");
		return;
	}
	mEqus.push_back(equ);
}
void EquationsCluster::add_var(var_ptr var)
{
	if(!var) {
		LOG_WARNING("Attempted to add null equation");
		return;
	}
	mVars.insert(var);
}

bool EquationsCluster::satisfied()
{
	for(auto equ : mEqus) {
		if(std::abs(equ->eval()) > 1e-12) // TODO: remove magic number and make it more formal
			return false;
	}
	return true;
}

int EquationsCluster::solve()
{
	int output = SolverState::FAILURE;

	clear_tags();
	clear_substitutions();
	substitutions();

	int single_tag = 1;
	for(auto equ : mEqus) {
		auto var = equ->get_single_var();
		if(var) {
			equ->set_tag(single_tag);
			output = solve_numeric(mAlgorithm, single_tag, true);
			if(output != SolverState::SUCCESS) {
				if(mVerboseLevel > 1)
					std::cout<<"Failed early at single tag = "<<single_tag<<"\n";
				output = SolverState::FAILURE;
				return output;
			}
			single_tag++;
		}
	}
	if(mVerboseLevel)
		std::cout<<"Solved "<<single_tag-1<<" equations alone\n";

	bool had_fixed_vars = false; // TODO: is this diminution thing still relevant?
	std::vector<int> diminutions(mVars.size(), 0);
	int n_diminutions = 0;

	do {
		output = solve_numeric(mAlgorithm, 0, true);
		
		if(output == SolverState::SUCCESS)
			break;
			
		had_fixed_vars = false;
		int i = 0;
		for(auto var : mVars) {
			if(var->as_coef_int() > 1) {
				had_fixed_vars = true;
				var->set_as_var();
				diminutions[i++]++;
			}
		}
		n_diminutions++;
	} while(had_fixed_vars);

	for(int i = 0; i < n_diminutions; ++i) {
		for(auto var : mVars) {
			if(diminutions[i]) {
				diminutions[i]--;
				var->set_as_coef();
			}
		}
	}
	return output;
}

int EquationsCluster::solve_numeric(int algo, int tag, bool drivingVars_only)
{
	switch(mAlgorithm) {
		case SolverState::DogLeg:
			return solve_DL(1e-14, tag, drivingVars_only);
			break;
		case SolverState::LevenbergMarquardt:
			return solve_LM(1e-24, tag, drivingVars_only);
			break;
		default:
			std::cout<<"Unknown solver "<<mAlgorithm<<"\n";
			return SolverState::FAILURE;
			break;
	}
}

void EquationsCluster::substitutions()
{
	for(auto equ : mEqus) {
		var_ptr a, b;
		if(equ->can_substitute()) {
			equ->get_substitution_vars(a, b);

			if(mVars.find(a) == mVars.end() || mVars.find(b) == mVars.end())
				continue;

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
	for(auto equ : mEqus) {
		if(equ->tag() == tag)
			errors(e++) = equ->eval();
	}
}
void EquationsCluster::compute_jacobi(Eigen::MatrixXd& jacobi, int tag, bool drivingVars_only)
{
	jacobi.resize(n_taggedEqus(tag), drivingVars_only ? n_drivingVars() : mVars.size());
	compute_jacobi_no_resize(jacobi, tag, drivingVars_only);
}
void EquationsCluster::compute_jacobi_no_resize(Eigen::MatrixXd& jacobi, int tag, bool drivingVars_only)
{
	int e = 0, v = 0;
	for(auto equ : mEqus) {
		if(equ->tag() != tag) 
			continue;
		for(auto var : mVars) {
			if(!drivingVars_only || !var->is_substituted()) {
				jacobi(e, v++) = equ->derivative(var)->eval();
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
int EquationsCluster::n_drivingVars()
{
	int n_vars_driving = 0;
	for(auto var : mVars) {
		if(!var->is_substituted())
			n_vars_driving++;
	}
	return n_vars_driving;
}

void EquationsCluster::incr_variables(Eigen::VectorXd const& delta, bool drivingVars_only) 
{
	int v = 0;
	for(auto var : mVars) {
		if(!drivingVars_only || !var->is_substituted())
			var->set(var->eval() + delta(v++));
	}
}

void EquationsCluster::set_variables(Eigen::VectorXd const& values, bool drivingVars_only)
{
	int v = 0;
	for(auto var : mVars) {
		if(!drivingVars_only || !var->is_substituted())
			var->set(values(v++));
	}
}

void EquationsCluster::retrieve_variables(Eigen::VectorXd& container, bool drivingVars_only) 
{
	int v = 0;
	for(auto var : mVars) {
		if(!drivingVars_only || !var->is_substituted())
			container(v++) = var->eval();
	}
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

int EquationsCluster::solve_LM(double eps1, int tag, bool drivingVars_only)
{
	int n_vars = drivingVars_only ? n_drivingVars() : mVars.size();		// Number of variables in the system
	int n_equs = n_taggedEqus(tag);	// Number of equations in the system
	if(mVerboseLevel > 1)
		std::cout<<"Num tagged: "<<n_equs<<"\n";
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
	retrieve_variables(P, drivingVars_only);
	compute_jacobi_no_resize(J, tag, drivingVars_only);
	compute_errors_no_resize(e, tag);
	e = -e;
	e_norm = e.squaredNorm();
	A = J.transpose() * J;
	g = J.transpose() * e;
	A_diag = A.diagonal();
	mu = tau * A_diag.lpNorm<Eigen::Infinity>();
	
	int output = g.lpNorm<Eigen::Infinity>() <= eps1 ? SolverState::SUCCESS : SolverState::RUNNING; // Check if the error is alreadu low enough
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
				set_variables(P_new, drivingVars_only);
				compute_errors_no_resize(e_new, tag);
				e_new = -e_new;
				e_new_norm = e_new.squaredNorm();

				rho = (e_norm - e_new_norm) / (dP.transpose() * (mu * dP + g)); // Compute rho (must be > 0 to escape the inner loop)
				if(rho > 0.0) {
					// Compute & retrieve new state
					P = P_new;
					compute_jacobi_no_resize(J, tag, drivingVars_only);
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
					A(i, i) = A_diag(i);
				}
				mu *= nu;
				nu *= 2.0;
			}
		}
	}

	set_variables(P, drivingVars_only); // TODO: is this assignment really necessary?
	if(mVerboseLevel) {
		switch(output) {
		case SolverState::SUCCESS:
			std::cout<<"LM solver faithful, succeeded in "<<k<<" iterations, finished with a squared error of "<<e_norm<<"\n";
			break;
		//case solveOutput::SolverState::FAILURE:
		default:
			std::cout<<"LM solver faithful, failed in "<<k<<" iterations, finished with a squared error of "<<e_norm<<"\n";
			break;
		}
	}
	return output;
}

int EquationsCluster::solve_DL(double eps, int tag, bool drivingVars_only)
{
	int n_vars = drivingVars_only ? n_drivingVars() : mVars.size();		// Number of variables in the system
	int n_equs = n_taggedEqus(tag);	// Number of equations in the system
	if(mVerboseLevel > 1)
		std::cout<<"Num tagged: "<<n_equs<<"\n";
	if(n_equs == 0)
		return SolverState::SUCCESS;
	Eigen::VectorXd X(n_vars), X_new(n_vars), X_init(n_vars),
					h_sd(n_vars), h_gn(n_vars), h_dl(n_vars), 	// Variables values, attempt change in variables and candidate variables values
					e(n_equs), e_new(n_equs), g(n_vars); 	// Error over variables, candidate variables and error scaled with jacobian  
	Eigen::MatrixXd J(n_equs, n_vars); 	// Jacobian matrix and approximation of Hessian matrix

	double r = 1.0;
	double eps1 = eps;
	double eps2 = eps;
	double eps3 = eps;
	
	// Compute and retrieve the initial state of the system
	retrieve_variables(X, drivingVars_only);
	X_init = X;
	compute_jacobi_no_resize(J, tag, drivingVars_only);
	compute_errors_no_resize(e, tag);

	g = J.transpose() * e;
	
	int output = (e.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? SolverState::SUCCESS : SolverState::RUNNING;
	int k;
	for(k = 0; k < mMaxIt_DL && !output; ++k) {
		double alpha = g.norm() / (J * g).norm();

		h_sd = -alpha * g;

		h_gn = J.fullPivLu().solve(-e); // Solve J(x) * h_gn = -f(x)

		if(h_gn.norm() <= r) {
			h_dl = h_gn;
		} else if((alpha * h_sd).norm() >= r) {
			h_dl = (r / h_sd.norm()) * h_sd;
		} else {
			Eigen::VectorXd a = alpha * h_sd, b = h_gn;
			double c = a.transpose() * (b - a);
			double beta;
			if(c <= 0) {
				beta = (-c + std::sqrt(c*c + (b-a).squaredNorm() * (r*r - a.squaredNorm()))) / (b-a).squaredNorm();
			} else {
				beta = (r*r - a.squaredNorm()) / (c + std::sqrt(c*c + (b-a).squaredNorm() * (r*r - a.squaredNorm())));
			}
			h_dl = a + beta * (b - a);
		}

		if(h_dl.norm() <= eps2 * (X.norm() + eps2)) {
			output = SolverState::SUCCESS;
			break;
		}

		X_new = X + h_dl;
		set_variables(X_new, drivingVars_only);
		compute_errors_no_resize(e_new, tag);

		double rho = (e.squaredNorm() / 2.0 - e_new.squaredNorm() / 2.0) / ((e.squaredNorm() / 2.0) - ((e + J * h_dl).squaredNorm() / 2.0));
		if(rho > 0) {
			X = X_new;
			e = e_new;
			compute_jacobi_no_resize(J, tag, drivingVars_only);
			g = J.transpose() * e;
			output = (e.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? SolverState::SUCCESS : SolverState::RUNNING;
		}
		if(rho > 0.75) {
			r = std::max(r, 3 * h_dl.norm());
		} else if(rho < 0.25) {
			r = r / 2.0;
			output = (r < eps2 * (X.norm() + eps2)) ? SolverState::FAILURE : SolverState::RUNNING;
		}
	}

	if(output != SolverState::SUCCESS) {
		set_variables(X_init, drivingVars_only);
	}

	if(mVerboseLevel) {
		switch(output) {
		case SolverState::SUCCESS:
			std::cout<<"DL solver, success. n: "<<k<<"; e: "<<e.squaredNorm()<<"; ID = "<<id()<<"\n";
			break;
		case SolverState::FAILURE:
		default:
			std::cout<<"DL solver, fail. n: "<<k<<"; e: "<<e.squaredNorm()<<"; ID = "<<id()<<"\n";
			break;
		}
	}
	return output;
}
