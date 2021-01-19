
#include <iostream>

#include <constraintsSolver/expression.hpp>
#include <constraintsSolver/equationsSystem.hpp>

#include <entities/geometry/point_abstract.hpp>
#include <utils/errorLogger.hpp>

#include <glm/gtx/string_cast.hpp>

int main()
{
	errorLogger::get_instance().init();

	point_abstract_ptr a = std::make_shared<point_abstract>(glm::vec3(-0.520555, 0.219129, 0)), b = std::make_shared<point_abstract>(glm::vec3(3, 4, 0));
	std::vector<expression_ptr> eqs {
		a->pos()->x->expr() - b->pos()->x->expr(),
		a->pos()->y->expr() - b->pos()->y->expr()
	};
	std::vector<variable_ptr> vars {
		a->pos()->x, a->pos()->y,
		b->pos()->x, b->pos()->y
	};
	equationsSystem sys(eqs, vars);

	std::cout<<"Start: "<<glm::to_string(a->pos_val())<<" - "<<glm::to_string(b->pos_val())<<"\n";
	std::cout<<"Sys size: "<<sys.vars_size()<<",  "<<sys.equs_size()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"End: "<<glm::to_string(a->pos_val())<<" - "<<glm::to_string(b->pos_val())<<"\n";

	
	return 0;
}

/*
	// Three variables, three equations


	variable_ptr x1(new variable("x1", 0.1, false));
	variable_ptr x2(new variable("x2", 0.1, false));
	variable_ptr x3(new variable("x3", -0.1, false));
	expression_ptr eq1 = 3 * x1->expr() - cos(x2->expr() * x3->expr()) - 0.5f;
	expression_ptr eq2 = pow(x1->expr(), 2.0f) - 81 * pow(x2->expr() + 0.1, 2.0f) + sin(x3->expr()) + 1.06;
	expression_ptr eq3 = exp(-x1->expr()*x2->expr()) + 20*x3->expr() + (10.0f * expConst::pi - 3.0f) / 3.0f;
	equationsSystem sys({ eq1, eq2, eq3 }, { x1, x2, x3 });
	
	std::cout<<"Sys size: "<<sys.vars_size()<<",  "<<sys.equs_size()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
	std::cout<<"Sys solve attempt: "<<sys.solve()<<"\n";
	std::cout<<"Sys solved: "<<sys.satisfied()<<"\n";
*/

/*
	// Two variables, one equation


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
	std::cout<<"Eq_i: "<<expr->eval()<<"\n";
	
	equationsSystem sys({ expr }, { x1, x2 });
	std::cout<<"Attempt solve: "<<sys.solve()<<"\n";
	std::cout<<"X1: "<<x1->val()<<"\n";
	std::cout<<"X2: "<<x2->val()<<"\n";
	std::cout<<"Eq_s: "<<expr->eval()<<"\n";
*/