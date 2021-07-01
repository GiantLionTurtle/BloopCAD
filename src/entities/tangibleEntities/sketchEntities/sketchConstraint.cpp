#include "sketchConstraint.hpp"

#include <constraintsSolver/expression.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>
#include <utils/errorLogger.hpp>
#include "sketchEntity.hpp"
#include <entities/sketch.hpp>
#include "spriteAnnotation.hpp"
#include <constraintsSolver/constraint.hpp>

#include <array>
#include <string>

constraint_entity::constraint_entity(std::vector<var_ptr> vars, std::vector<equ_ptr> equs):
	entity(),
	mVars(vars),
	mEqus(equs)
{
	set_name("constraint");
}

double constraint_entity::error(size_t eq) 
{
	return mEqus[eq]->eval();
}
bool constraint_entity::satisfied() 
{
	for(size_t i = 0; i < mEqus.size(); ++i) {
		if(std::abs(mEqus[i]->eval()) > 1e-12)
			return false;
	}
	return true;
}
double constraint_entity::derivative(var_ptr withRespectTo, size_t from_eq)
{
	// TODO: would going through the constraint's variables to check if the target
	// is in the constraint save time? ; output zero if the variable is unknown to this constraint
	withRespectTo->set_as_var();
	double der = mEqus[from_eq]->d()->eval();
	withRespectTo->set_as_coef();
	return der;
}

void constraint_entity::exists_impl(bool ex)
{
	// The variables are not toggled as other constraints might use them
	for(auto equ : mEqus)
		equ->set_exists(ex);
}

pointPoint_horizontality::pointPoint_horizontality(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p1, sketchPoint_ptr p2):
	sprite_constraint<2>({ }, { p1->y() ^= p2->y() }, baseplane_,
	"resources/textures/images/icons/sketch/constraints/horizontality.png"),
	mP1(p1),
	mP2(p2)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->y());
	if(!mP2->fixed())
		mVars.push_back(mP2->y());

	mP1->add_floatingAnnotation(mAnnots[0]);
	mP2->add_floatingAnnotation(mAnnots[1]);
}

line_horizontality::line_horizontality(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l):
	sprite_constraint<1>({ }, { l->A()->y() ^= l->B()->y() }, baseplane_,
	"resources/textures/images/icons/sketch/constraints/horizontality.png"),
	mLine(l)
{
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->A()->y(), l->B()->y() });
	mLine->add_floatingAnnotation(mAnnots[0]);
}


pointPoint_verticality::pointPoint_verticality(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p1, sketchPoint_ptr p2):
	sprite_constraint<2>({ }, { p1->x() ^= p2->x() }, baseplane_,
	"resources/textures/images/icons/sketch/constraints/verticality.png"),
	mP1(p1),
	mP2(p2)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->x());
	if(!mP2->fixed())
		mVars.push_back(mP2->x());
	mP1->add_floatingAnnotation(mAnnots[0]);
	mP2->add_floatingAnnotation(mAnnots[1]);
}

line_verticality::line_verticality(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l):
	sprite_constraint<1>({ }, { l->A()->x() ^= l->B()->x() }, baseplane_,
	"resources/textures/images/icons/sketch/constraints/verticality.png"),
	mLine(l)
{
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->A()->x(), l->B()->x() });
	mLine->add_floatingAnnotation(mAnnots[0]);
}

line_length::line_length(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l, expression_ptr length):
	constraint_entity({ }, 
	{ pow(l->A()->x() - l->B()->x(), 2.0) + pow(l->A()->y() - l->B()->y(), 2.0) ^= pow(length, 2.0) }),
	mLine(l),
	mLength(length)
{
	// Equation: pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0)

	// The square distance between two points is pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0)
	// It must be equal to the square of d
	// TODO: check if squaring d truly is more efficient than taking the square root of the actual distance
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y() });
}

pointPoint_horizontalDistance::pointPoint_horizontalDistance(geom_3d::plane_abstr_ptr baseplane_, 
sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr dist):
	constraint_entity({ }, { abs(p1->x() - p2->x()) ^= dist }),
	mP1(p1),
	mP2(p2),
	mDistance(dist)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->x());
	if(!mP2->fixed())
		mVars.push_back(mP2->x());
	
}

pointPoint_verticalDistance::pointPoint_verticalDistance(geom_3d::plane_abstr_ptr baseplane_, 
sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr dist):
	constraint_entity({ p1->y(), p2->y() }, { abs(p1->y() - p2->y()) ^= dist }),
	mP1(p1),
	mP2(p2),
	mDistance(dist)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->y());
	if(!mP2->fixed())
		mVars.push_back(mP2->y());	
}

pointLine_distance_abstr::pointLine_distance_abstr(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p, sketchLine_ptr l, expression_ptr dist):
	constraint_entity({ },
	{ abs((l->B()->x()-l->A()->x())*(p->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(p->x() - l->A()->x())) / sqrt(l->length2()) ^= dist })
{
	if(!p->fixed())
		mVars.insert(mVars.end(), { p->x(), p->y() });
	if(!l->fixed())
		mVars.insert(mVars.end(), { l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y() });
}


pointLine_distance::pointLine_distance(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p, sketchLine_ptr l, expression_ptr dist):
	pointLine_distance_abstr(baseplane_, p, l, dist),
	mPoint(p),
	mLine(l),
	mDistance(dist)
{

}

circleLine_distance::circleLine_distance(geom_3d::plane_abstr_ptr baseplane_, sketchCircle_ptr c, sketchLine_ptr l, expression_ptr dist):
	pointLine_distance_abstr(baseplane_, c->center(), l, dist + c->radius())
{

}

lineLinePerpendicularity::lineLinePerpendicularity(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l1, sketchLine_ptr l2):
	sprite_constraint<2>({ }, { }, baseplane_, "resources/textures/images/icons/sketch/constraints/perpendicularity.png"),
	mLine1(l1),
	mLine2(l2)
{
	if(!l1->fixed())
		mVars.insert(mVars.end(), { l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->y() });
	if(!l2->fixed())
		mVars.insert(mVars.end(), { l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() });

	expression_ptr x1 = (l1->A()->x()-l1->B()->x());
	expression_ptr y1 = (l1->A()->y()-l1->B()->y());
	expression_ptr x2 = (l2->A()->x()-l2->B()->x());
	expression_ptr y2 = (l2->A()->y()-l2->B()->y());

	mEqus.push_back(mod(acos(dot(x1, y1, x2, y2) / (sqrt(x1*x1+y1*y1) * sqrt(x2*x2+y2*y2))), 2.0*M_PI) ^= mod(expConst::pi2, 2.0*M_PI));

	// https://www.omnicalculator.com/math/angle-between-two-vectors

	mLine1->add_floatingAnnotation(mAnnots[0]);
	mLine2->add_floatingAnnotation(mAnnots[1]);
}

pointPoint_coincidence::pointPoint_coincidence(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p1, sketchPoint_ptr p2):
	sprite_constraint<1>({ }, { p1->x() ^= p2->x(), p1->y() ^= p2->y()}, baseplane_, 
	"resources/textures/images/icons/sketch/constraints/coincidence.png"),
	mP1(p1),
	mP2(p2)
{
	if(!mP1->fixed())
		mVars.insert(mVars.end(), { mP1->x(), mP1->y() });
	if(!mP2->fixed())
		mVars.insert(mVars.end(), { mP2->x(), mP2->y() });
	
	mP1->add_floatingAnnotation(mAnnots[0]);
	mP2->add_floatingAnnotation(mAnnots[1]);	
}
