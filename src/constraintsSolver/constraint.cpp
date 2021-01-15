
#include "constraint.hpp"

#include <entities/entity.hpp>

constraint::constraint()
{

}
constraint::constraint(std::vector<expression_ptr> equs, std::vector<variable_ptr> vars):
	mEquations(equs),
	mVariables(vars)
{

}

coincidence_constraint::coincidence_constraint(point_abstract_ptr p1, point_abstract_ptr p2):
	mP1(p1),
	mP2(p2),
	mMerged(false)
{
	mEquations = { p1->pos_var()->x->expr() - p2->pos_var()->x->expr(), p1->pos_var()->y->expr() - p2->pos_var()->y->expr() };
	mVariables = { p1->pos_var()->x, p1->pos_var()->y, p2->pos_var()->x, p2->pos_var()->y };
}

void coincidence_constraint::on_added()
{
	mMerged = false;
	entity_ptr to_hide = std::dynamic_pointer_cast<entity>(mP1);
	if(!to_hide) {
		to_hide = std::dynamic_pointer_cast<entity>(mP2);
		if(!to_hide)
			return;
	}
	mMerged = true;
	to_hide->hide();
}
void coincidence_constraint::on_removed()
{
	if(!mMerged) 
		return;
	entity_ptr to_show = std::dynamic_pointer_cast<entity>(mP1);
	if(!to_show) {
		to_show = std::dynamic_pointer_cast<entity>(mP2);
		if(!to_show)
			return;
	}
	mMerged = false;
	to_show->hide();
}