
#include "constraint.hpp"

#include <entities/entity.hpp>

constraint::constraint()
{

}
constraint::constraint(std::vector<expression_ptr> equs, std::vector<variable_ptr> vars):
	mVariables(vars),
	mEquations(equs)
{

}
constraint::constraint(subEquationsSystem const& sys):
	mVariables(sys.variables),
	mEquations(sys.equations)
{

}

// coincidence_constraint::coincidence_constraint(point_abstract_ptr p1, point_abstract_ptr p2):
// 	mP1(p1),
// 	mP2(p2),
// 	mMerged(false)
// {
// 	mEquations = { p1->pos()->x->expr() - p2->pos()->x->expr(), p1->pos()->y->expr() - p2->pos()->y->expr() };
// 	mVariables = { p1->pos()->x, p1->pos()->y, p2->pos()->x, p2->pos()->y };
// }