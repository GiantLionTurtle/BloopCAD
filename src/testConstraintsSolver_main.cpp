
#include <iostream>

#include <constraintsSolver/expression.hpp>
#include <constraintsSolver/equationsSystem.hpp>

#include <utils/errorLogger.hpp>

int main()
{
	errorLogger::get_instance().init();

	expression_ptr x1(new expression_variable(variable_ptr(new variable("x1"))));
	expression_ptr x2(new expression_variable(variable_ptr(new variable("x2"))));
	expression_ptr x3(new expression_variable(variable_ptr(new variable("x3"))));
	equationsSystem sys({
		3 * x1 - cos(x2 * x3) - 0.5f,
		pow(x1, 2.0f) - 81 * pow(x2 + 0.1, 2.0f) + sin(x3) + 1.06,
		exp(-x1*x2) + 20*x3 + (10.0f * expConst::pi - 3.0f) / 3.0f });
	
	std::cout<<"Sys size: "<<sys.size()<<"\n";
	std::cout<<"der: "<<(exp(-x1*x2) + 20*x3 + (10.0f * expConst::pi - 3.0f) / 3.0f)->derivative()<<"\n";
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