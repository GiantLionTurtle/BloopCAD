
#include <iostream>

#include <constraintsSolver/expression.hpp>

int main()
{
	variable_ptr x(new variable("x", 4.5));
	expression_ptr v(new expression_const(5.6));
	expression_ptr w(new expression_const(4.4));

	expression_ptr exp = sin(pow(x->expr(), 2.0f));// * sqrt(v) + cot(v) / -v;
	std::cout<<"Exp: "<<exp->to_string()<<"\n";
	std::cout<<"Exp eval: "<<exp->eval()<<"\n";
	std::cout<<"Exp derivative: "<<exp->derivative()<<"\n";
	std::cout<<"Exp derivative eval: "<<exp->derivative()->eval()<<"\n";
	return 0;
}