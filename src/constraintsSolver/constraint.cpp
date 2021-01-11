
#include "constraint.hpp"

constraint::constraint()
{

}
constraint::constraint(std::vector<expression_ptr> equs, std::vector<variable_ptr> vars):
    mEquations(equs),
    mVariables(vars)
{

}

coincidence_constraint::coincidence_constraint(point_abstract_ptr p1, point_abstract_ptr p2)
{
    mEquations = { p1->pos_var()->x->expr() - p2->pos_var()->x->expr(), p1->pos_var()->y->expr() - p2->pos_var()->y->expr() };
    mVariables = { p1->pos_var()->x, p1->pos_var()->y };//, p2->pos_var()->x, p2->pos_var()->y };
}
