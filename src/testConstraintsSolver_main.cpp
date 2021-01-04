
#include <iostream>

#include <constraintsSolver/expression.hpp>

int main()
{
	expression_ptr v(new expression_const(5.6));
	expression_ptr w(new expression_const(4.4));

	std::cout<<"v: "<<v->to_string()<<"\n";
	std::cout<<"w: "<<w->to_string()<<"\n";
	std::cout<<"v+w: "<<(v+w)->to_string()<<" => "<<(v+w)->eval()<<"\n";
	std::cout<<"v-w: "<<(v-w)->to_string()<<" => "<<(v-w)->eval()<<"\n";

	return 0;
}