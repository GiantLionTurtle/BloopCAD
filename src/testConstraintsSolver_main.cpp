
#include <iostream>

#include <constraintsSolver/expression.hpp>
#include <constraintsSolver/equationsSystem.hpp>

#include <entities/geometry/point_abstract.hpp>
#include <utils/errorLogger.hpp>

int main()
{
	errorLogger::get_instance().init();

	// point_abstract_ptr a = std::make_shared<point_abstract>(glm::vec3(-0.520555, 0.219129, 0)), b = std::make_shared<point_abstract>(glm::vec3(3, 4, 0));
	// std::vector<expression_ptr> eqs {
	// 	a->pos_var()->x->expr() - b->pos_var()->x->expr(),
	// 	a->pos_var()->y->expr() - b->pos_var()->y->expr()
	// };
	// std::vector<variable_ptr> vars {
	// 	a->pos_var()->x, a->pos_var()->y,
	// 	b->pos_var()->x, b->pos_var()->y
	// };
	// equationsSystem sys(eqs, vars);

	// std::cout<<"Sys size: "<<sys.size()<<"\n";
	// std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	// std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	// std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";

	variable_ptr x1 = std::make_shared<variable>("x1", 0.0f), x2 = std::make_shared<variable>("x2", 0.0f);
	x1->set_fixed(true);
	x2->set_fixed(true);
	expression_ptr expr = pow(pow(x1->expr(), 2.0f) + x2->expr() - 11.0f, 2.0f) + pow(x1->expr() + pow(x2->expr(), 2.0f) - 7.0f, 2.0f);

	std::cout<<"Expr: "<<expr<<"\n";
	x1->set_fixed(false);
	std::cout<<"DX1: "<<expr->d()->eval()<<"\n";
	x1->set_fixed(true);
	x2->set_fixed(false);
	std::cout<<"DX1: "<<expr->d()->eval()<<"\n";
	return 0;
}

