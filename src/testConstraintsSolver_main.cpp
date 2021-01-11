
#include <iostream>

#include <constraintsSolver/expression.hpp>
#include <constraintsSolver/equationsSystem.hpp>

#include <entities/geometry/point_abstract.hpp>
#include <utils/errorLogger.hpp>

int main()
{
	errorLogger::get_instance().init();

	point_abstract_ptr a = std::make_shared<point_abstract>(glm::vec3(-0.520555, 0.219129, 0)), b = std::make_shared<point_abstract>(glm::vec3(3, 4, 0));
	std::vector<expression_ptr> eqs {
		a->pos_var()->x->expr() - b->pos_var()->x->expr(),
		a->pos_var()->y->expr() - b->pos_var()->y->expr()
	};
	std::vector<variable_ptr> vars {
		a->pos_var()->x, a->pos_var()->y
	};
	equationsSystem sys(eqs, vars);

	std::cout<<"Sys size: "<<sys.size()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";

	return 0;
}

