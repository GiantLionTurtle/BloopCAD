
#include <iostream>

#include <constraintsSolver/expression.hpp>
#include <constraintsSolver/equationsSystem.hpp>

#include <utils/errorLogger.hpp>

int main()
{
	errorLogger::get_instance().init();

	variable_ptr x1(new variable("x1", 0.1, false));
	variable_ptr x2(new variable("x2", 0.1, false));
	variable_ptr x3(new variable("x3", -0.1, false));
	expression_ptr eq1 = 3 * x1->expr() - cos(x2->expr() * x3->expr()) - 0.5f;
	expression_ptr eq2 = pow(x1->expr(), 2.0f) - 81 * pow(x2->expr() + 0.1, 2.0f) + sin(x3->expr()) + 1.06;
	expression_ptr eq3 = exp(-x1->expr()*x2->expr()) + 20*x3->expr() + (10.0f * expConst::pi - 3.0f) / 3.0f;
	equationsSystem sys({ eq1, eq2, eq3 }, { x1, x2, x3 });
	
	std::cout<<"Sys size: "<<sys.size()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	// std::cout<<"Eq1: "<<eq1<<"\n";
	// x1->set_fixed(true);
	// x2->set_fixed(false);
	// x3->set_fixed(true);
	// std::cout<<"Eq1d: "<<eq2->d()->eval()<<"\n";
	/*
	variable_ptr x(new variable("x", 4.5));
	expression_ptr exp = sin(pow(x->expr(), 2.0f));// * sqrt(v) + cot(v) / -v;
	std::cout<<"Exp: "<<exp->to_string()<<"\n";
	std::cout<<"Exp eval: "<<exp->eval()<<"\n";
	std::cout<<"Exp derivative: "<<exp->derivative()<<"\n";
	std::cout<<"Exp derivative eval: "<<exp->derivative()->eval()<<"\n";
	*/
	return 0;
}

