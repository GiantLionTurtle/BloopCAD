
#include "constraintCluster.hpp"

#include "constraintSystem.hpp"

#include <iostream>

constraintCluster::constraintCluster(std::vector<std::shared_ptr<constraint_abstract>> constraints, std::vector<variable_ptr> vars, int solver_algo, int verbose):
	mConstraints(constraints),
	mVariables(vars),
	mVerboseLevel(verbose),
	mId(0),
	mAlgorithm(solver_algo)
{

}
constraintCluster::~constraintCluster()
{

}
bool constraintCluster::satisfied()
{
	for(std::shared_ptr<constraint_abstract> constr : mConstraints) {
		if(!constr->satisfied())
			return false;
	}
	return true;
}

int constraintCluster::solve()
{
	int output = FAILURE;
	int diminutions = 0;
	bool had_fixed_vars = false;
	do {
		switch(mAlgorithm) {
			case constraintSystem::DogLeg:
				output = solve_DL();
				break;
			case constraintSystem::LevenbergMarquardt:
				output = solve_LM_faithful();//solve_LM2();
				break;
			default:
				std::cout<<"Unknown solver "<<mAlgorithm<<"\n";
				return FAILURE;
				break;
		}
		if(output == SUCCESS)
			break;
		
		had_fixed_vars = false;
		for(variable_ptr var : mVariables) {
			if(var->as_coef()) {
				had_fixed_vars = true;
			}
			var->set_as_var();
		}
		diminutions++;
	} while(had_fixed_vars);

	for(int i = 0; i < diminutions; ++i) {
		for(variable_ptr var : mVariables)
			var->set_as_coef();
	}
	return output;
}

int constraintCluster::solve_LM() 
{
	// Save the initial values of the variables in case solve fails
	Eigen::VectorXd init_vars(mVariables.size());
	retrieve_variables(init_vars);
	int output = solve_LM_naive();
	if(output != solveOutput::SUCCESS) // Reset variables on naive solve fail
		set_variables(init_vars);
	output = solve_LM_faithful();
	// if(output != solveOutput::SUCCESS) // Reset variables on faithful solve fail
		// set_variables(init_vars);
	return output;
}

int constraintCluster::solve_LM_faithful(double eps1)
{
	int n_vars = mVariables.size();		// Number of variables in the system
	int n_cons = mConstraints.size();	// Number of equations in the system
	Eigen::VectorXd P(n_vars), dP(n_vars), P_new(n_vars), 	// Variables values, attempt change in variables and candidate variables values
					e(n_cons), e_new(n_cons), g(n_vars), 	// Error over variables, candidate variables and error scaled with jacobian  
					A_diag(n_vars);							// Diagonal of approximation of Hessian matrix
	Eigen::MatrixXd J(n_cons, n_vars), A(n_vars, n_vars); 	// Jacobian matrix and approximation of Hessian matrix

	double eps2 = 1e-24; 		// Is part of a change threshold that determines if the solver is stuck
	double tau = 1e-3;			// Serves as a basis for the meta parameter mu
	double mu, nu = 2.0; 		// Meta parameters defining the aggressivity of the steps
	double e_norm, e_new_norm;	// Squared norm of the error on the variables and the candidate variables
	double g_norm;				// Infinity norm of the profuct J.inv * e
	
	// Compute and retrieve the initial state of the system
	retrieve_variables(P);
	compute_jacobi(J);
	compute_errors(e);
	e = -e;
	e_norm = e.squaredNorm();
	A = J.transpose() * J;
	g = J.transpose() * e;
	A_diag = A.diagonal();
	mu = tau * A_diag.lpNorm<Eigen::Infinity>();
	
	int output = g.lpNorm<Eigen::Infinity>() <= eps1 ? solveOutput::SUCCESS : solveOutput::RUNNING; // Check if the error is alreadu low enough
	int k;
	for(k = 0; k < mMaxIt_LM && !output; ++k) {
		if(e_norm <= eps1) { // Error is low enough to exit
			output = solveOutput::SUCCESS;
			break;
		}
		if(e_norm != e_norm) { // NaN, exit
			output = solveOutput::FAILURE;
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
					output = solveOutput::FAILURE;
					break;
				}


				// Assign new values to the candidate values and compute error
				P_new = P + dP;
				set_variables(P_new);
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
					e = e_new;
					e_norm = e_new_norm;
					g = J.transpose() * e;

					// Check if the error is low enough
					output = g.lpNorm<Eigen::Infinity>() <= eps1 ? solveOutput::SUCCESS : solveOutput::RUNNING;
					
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

	set_variables(P); // TODO: is this assignment really necessary?
	if(mVerboseLevel) {
		switch(output) {
		case solveOutput::SUCCESS:
			std::cout<<"LM solver faithful, succeeded in "<<k<<" iterations, finished with a squared error of "<<e_norm<<"\n";
			break;
		//case solveOutput::FAILURE:
		default:
			std::cout<<"LM solver faithful, failed in "<<k<<" iterations, finished with a squared error of "<<e_norm<<"\n";
			break;
		}
	}
	return output;
}

int constraintCluster::solve_LM_naive(double eps1)
{
	int n_vars = mVariables.size(); 		// Number of variables in the system
	int n_cons = mConstraints.size();		// Number of equations in the system
	Eigen::VectorXd dP(n_vars), e(n_cons);	// Change in variables at an iteration and error in the equations at an iteration
	Eigen::MatrixXd A(n_vars, n_vars), J(n_cons, n_vars); // Aprox of Hessian matrix and jacombian matric
	
	compute_errors(e); // Finf the error
	double e_norm = e.squaredNorm();
	double prev_e_norm = e_norm; // prev_e_norm is used to know it the iteration improved or worsened the solve
	
	// initial hessian approx and jacobian
	compute_jacobi(J);
	A = J.transpose() * J;
	double lambda = 1e-3 * A.diagonal().lpNorm<Eigen::Infinity>(); // Scale the lambda parameter with the initial state	

	int output = solveOutput::RUNNING;
	int k;
	for(k = 0; k< mMaxIt_LM && !output; ++k) {
		if(e_norm != e_norm) { // Check for NaN
			break;
		}
		if(e_norm <= eps1) { // Check for success
			output = solveOutput::SUCCESS;
			break;
		}

		// change in variables = (A + lambda * I).inv * g
		for(int j = 0; j < n_vars; ++j) { // this loop augments A's diagonal
			A(j, j) += lambda;
		}
		dP = -A.inverse() * (J.transpose() * e);
		
		incr_variables(dP); // New and improved/worsened variables!

		compute_errors(e); // Just how much did the parameter helped?
		e_norm = e.squaredNorm();
		if(e_norm < prev_e_norm) { // They did help! => reduce lambda
			lambda *= 0.5;
		} else { // They did not.. => Augment lambda
			lambda *= 2;
		}
		prev_e_norm = e_norm; // Remember this iteration's error

		// Compute the new jacobi and the hessian approx
		compute_jacobi(J);
		A = J.transpose() * J;
	}

	if(mVerboseLevel) {
		switch(output) {
		case solveOutput::SUCCESS:
			std::cout<<"LM solver naive, succeeded in "<<k<<" iterations, finished with a squared error of "<<e_norm<<"\n";
			break;
		// case solveOutput::FAILURE:
		default:
			std::cout<<"LM solver naive, failed in "<<k<<" iterations, finished with a squared error of "<<e_norm<<"\n";
			break;
		}
	}
	return output;
}

int constraintCluster::solve_LM2(double eps)
{
	int n_vars = mVariables.size();		// Number of variables in the system
	int n_cons = mConstraints.size();	// Number of equations in the system
	Eigen::VectorXd X(n_vars), h_lm(n_vars), X_new(n_vars), 	// Variables values, attempt change in variables and candidate variables values
					e(n_cons), e_new(n_cons), g(n_vars), 	// Error over variables, candidate variables and error scaled with jacobian  
					A_diag(n_vars);							// Diagonal of approximation of Hessian matrix
	Eigen::MatrixXd J(n_cons, n_vars), A(n_vars, n_vars); 	// Jacobian matrix and approximation of Hessian matrix

	double eps1 = eps;
	double eps2 = eps;
	double tau = 1e-3;			// Serves as a basis for the meta parameter mu
	double mu, nu = 2.0; 		// Meta parameters defining the aggressivity of the steps

	// Compute and retrieve the initial state of the system
	retrieve_variables(X);
	compute_jacobi(J);
	compute_errors(e);

	g = J.transpose() * e;
	A = J.transpose() * J;

	int output = g.lpNorm<Eigen::Infinity>() <= eps1 ? solveOutput::SUCCESS : solveOutput::RUNNING;
	mu = tau * A.diagonal().maxCoeff();

	int k;
	for(k = 0; k < mMaxIt_LM && !output; ++k) {
		A_diag = A.diagonal();
		for(int i = 0; i < n_vars; ++i) {
			A(i, i) += mu;
		}
		h_lm = A.fullPivLu().solve(-g);

		if(h_lm.norm() <= eps2 * (X.norm() + eps2)) {
			output = solveOutput::SUCCESS;
			break;
		}

		X_new = X + h_lm;
		set_variables(X_new);
		compute_errors(e_new);

		double rho = (e.squaredNorm() / 2.0 - e_new.squaredNorm() / 2.0) / ((e.squaredNorm() / 2.0) - ((e + J * h_lm).squaredNorm() / 2.0));

		if(rho > 0) {
			X = X_new;
			e = e_new;
			compute_jacobi(J);
			g = J.transpose() * e;
			A = J.transpose() * J;
			output = g.lpNorm<Eigen::Infinity>() <= eps1 ? solveOutput::SUCCESS : solveOutput::RUNNING;

			double tmp = 2 * rho - 1;
			mu *= std::max(1.0 / 3.0, 1.0 - tmp*tmp*tmp);
			nu  = 2;
		} else {
			mu *= nu;
			nu *= 2;

			for(int i = 0; i < n_vars; ++i) {
				A(i, i) = A_diag(i);
			}
		}
	}

	if(mVerboseLevel) {
		switch(output) {
		case solveOutput::SUCCESS:
			std::cout<<"LM2 solver, succeeded in "<<k<<" iterations, finished with a squared error of "<<e.squaredNorm()<<"\n";
			break;
		//case solveOutput::FAILURE:
		default:
			std::cout<<"LM2 solver, failed in "<<k<<" iterations, finished with a squared error of "<<e.squaredNorm()<<"\n";
			break;
		}
	}
	return output;
}

int constraintCluster::solve_DL(double eps)
{
	int n_vars = mVariables.size();		// Number of variables in the system
	int n_cons = mConstraints.size();	// Number of equations in the system
	Eigen::VectorXd X(n_vars), X_new(n_vars), X_init(n_vars),
					h_sd(n_vars), h_gn(n_vars), h_dl(n_vars), 	// Variables values, attempt change in variables and candidate variables values
					e(n_cons), e_new(n_cons), g(n_vars); 	// Error over variables, candidate variables and error scaled with jacobian  
	Eigen::MatrixXd J(n_cons, n_vars); 	// Jacobian matrix and approximation of Hessian matrix


	double r = 1.0;
	double eps1 = eps;
	double eps2 = eps;
	double eps3 = eps;
	
	// Compute and retrieve the initial state of the system
	retrieve_variables(X);
	X_init = X;
	compute_jacobi(J);
	compute_errors(e);

	g = J.transpose() * e;
	
	int output = (e.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? solveOutput::SUCCESS : solveOutput::RUNNING;
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
			output = solveOutput::SUCCESS;
			break;
		}

		X_new = X + h_dl;
		set_variables(X_new);
		compute_errors(e_new);

		double rho = (e.squaredNorm() / 2.0 - e_new.squaredNorm() / 2.0) / ((e.squaredNorm() / 2.0) - ((e + J * h_dl).squaredNorm() / 2.0));
		if(rho > 0) {
			X = X_new;
			e = e_new;
			compute_jacobi(J);
			output = (e.lpNorm<Eigen::Infinity>() <= eps3 || g.lpNorm<Eigen::Infinity>() <= eps1) ? solveOutput::SUCCESS : solveOutput::RUNNING;
		}
		if(rho > 0.75) {
			r = std::max(r, 3 * h_dl.norm());
		} else if(rho < 0.25) {
			r = r / 2.0;
			output = (r < eps2 * (X.norm() + eps2)) ? solveOutput::FAILURE : solveOutput::RUNNING;
		}
	}

	if(e.squaredNorm() >= eps*eps)
		output = solveOutput::FAILURE;

	if(output != solveOutput::SUCCESS) {
		set_variables(X_init);
	}

	if(mVerboseLevel) {
		int num0 = 0;
		for(int i = 0; i < mVariables.size(); ++i) {
			if(mVariables[i]->as_coef_int() > 1)
				num0 ++;
		}

		switch(output) {
		case solveOutput::SUCCESS:
			std::cout<<"DL solver, succeeded in "<<k<<" iterations, finished with a squared error of "<<e.squaredNorm()
			<<" and "<<num0<<" fixed vars ("<<mVariables.size()<<" vars in total) clusterID = "<<id()<<"\n";
			break;
		//case solveOutput::FAILURE:
		default:
			std::cout<<"DL solver, failed in "<<k<<" iterations, finished with a squared error of "<<e.squaredNorm()
			<<" and "<<num0<<" fixed vars ("<<mVariables.size()<<" vars in total) clusterID = "<<id()<<"\n";
			break;
		}
	}
	return output;
}

void constraintCluster::compute_errors(Eigen::VectorXd& errors)
{
	errors.resize(mConstraints.size());
	for(int i = 0; i < mConstraints.size(); ++i) {
		errors(i) = mConstraints[i]->error();
	}
}

void constraintCluster::compute_jacobi(Eigen::MatrixXd& jacobi)
{
	jacobi.resize(mConstraints.size(), mVariables.size());
	for(int i = 0; i < mConstraints.size(); ++i) {
		for(int j = 0; j < mVariables.size(); ++j) {
			jacobi(i, j) = mConstraints[i]->derivative(mVariables[j]);
		}
	}
}

void constraintCluster::incr_variables(Eigen::VectorXd const& delta) 
{
	for(int i = 0; i < mVariables.size(); ++i) {
		mVariables[i]->set(mVariables[i]->eval() + delta(i));
	}
}

void constraintCluster::set_variables(Eigen::VectorXd const& values)
{
	for(int i = 0; i < mVariables.size(); ++i) {
		mVariables[i]->set(values(i));
	}
}

void constraintCluster::retrieve_variables(Eigen::VectorXd& container) 
{
	container.resize(mVariables.size());
	for(int i = 0; i < mVariables.size(); ++i) {
		container(i) = mVariables[i]->eval();
	}	
}