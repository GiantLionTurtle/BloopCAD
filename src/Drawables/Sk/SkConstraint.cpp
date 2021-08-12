
#include "SkConstraint.hpp"

#include <ConstraintsSolver/Expression.hpp>
#include "SkPoint.hpp"
#include "SkLine.hpp"

#include <utils/DebugUtils.hpp>
#include <Drawables/Sketch.hpp>
#include <ConstraintsSolver/Constraint_abstr.hpp>

#include <array>
#include <string>

SkConstraint::SkConstraint(Geom3d::plane_abstr* baseplane_, std::vector<var_ptr> vars, std::vector<equ_ptr> equs):
	SkDrawable(baseplane_),
	mVars(vars),
	mEqus(equs)
{
	set_name("constraint");
}

double SkConstraint::error(size_t eq) 
{
	return mEqus[eq]->eval();
}
bool SkConstraint::satisfied() 
{
	for(size_t i = 0; i < mEqus.size(); ++i) {
		if(std::abs(mEqus[i]->eval()) > 1e-12)
			return false;
	}
	return true;
}
double SkConstraint::derivative(var_ptr withRespectTo, size_t from_eq)
{
	// TODO: would going through the constraint's variables to check if the target
	// is in the constraint save time? ; output zero if the variable is unknown to this constraint
	withRespectTo->set_as_var();
	double der = mEqus[from_eq]->d()->eval();
	withRespectTo->set_as_coef();
	return der;
}

void SkConstraint::exists_impl(bool ex)
{
	// The variables are not toggled as other constraints might use them
	for(auto equ : mEqus)
		equ->set_exists(ex);
}

SkPointPoint_horizontality::SkPointPoint_horizontality(Geom3d::plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
	SkSprite_constraint<2>(baseplane_, { }, { p1->y() ^= p2->y() },
	"resources/textures/images/icons/sketch/constraints/horizontality.png"),
	mP1(p1),
	mP2(p2)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->y());
	if(!mP2->fixed())
		mVars.push_back(mP2->y());

	mP1->add_annot(annot(0));
	mP2->add_annot(annot(1));
}

SkLine_horizontality::SkLine_horizontality(Geom3d::plane_abstr* baseplane_, SkLineCurve* l):
	SkSprite_constraint<1>(baseplane_, { }, { l->ptA()->y() ^= l->ptB()->y() },
	"resources/textures/images/icons/sketch/constraints/horizontality.png"),
	mLine(l)
{
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->ptA()->y(), l->ptB()->y() });
	mLine->add_annot(annot(0));
}


SkPointPoint_verticality::SkPointPoint_verticality(Geom3d::plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
	SkSprite_constraint<2>(baseplane_, { }, { p1->x() ^= p2->x() },
	"resources/textures/images/icons/sketch/constraints/verticality.png"),
	mP1(p1),
	mP2(p2)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->x());
	if(!mP2->fixed())
		mVars.push_back(mP2->x());
	mP1->add_annot(annot(0));
	mP2->add_annot(annot(1));
}

SkLine_verticality::SkLine_verticality(Geom3d::plane_abstr* baseplane_, SkLineCurve* l):
	SkSprite_constraint<1>(baseplane_, { }, { l->ptA()->x() ^= l->ptB()->x() },
	"resources/textures/images/icons/sketch/constraints/verticality.png"),
	mLine(l)
{
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptB()->x() });
	mLine->add_annot(annot(0));
}

SkLine_length::SkLine_length(Geom3d::plane_abstr* baseplane_, SkLineCurve* l, exp_ptr length):
	SkConstraint(baseplane_, { }, 
	{ pow(l->ptA()->x() - l->ptB()->x(), 2.0) + pow(l->ptA()->y() - l->ptB()->y(), 2.0) ^= pow(length, 2.0) }),
	mLine(l),
	mLength(length)
{
	// Equation: pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0)

	// The square distance between two points is pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0)
	// It must be equal to the square of d
	// TODO: check if squaring d truly is more efficient than taking the square root of the actual distance
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
}

SkPointPoint_horizontalDistance::SkPointPoint_horizontalDistance(Geom3d::plane_abstr* baseplane_, 
SkPoint* p1, SkPoint* p2, exp_ptr dist):
	SkConstraint(baseplane_, { }, { abs(p1->x() - p2->x()) ^= dist }),
	mP1(p1),
	mP2(p2),
	mDistance(dist)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->x());
	if(!mP2->fixed())
		mVars.push_back(mP2->x());
	
}

SkPointPoint_verticalDistance::SkPointPoint_verticalDistance(Geom3d::plane_abstr* baseplane_, 
SkPoint* p1, SkPoint* p2, exp_ptr dist):
	SkConstraint(baseplane_, { p1->y(), p2->y() }, { abs(p1->y() - p2->y()) ^= dist }),
	mP1(p1),
	mP2(p2),
	mDistance(dist)
{
	if(!mP1->fixed())
		mVars.push_back(mP1->y());
	if(!mP2->fixed())
		mVars.push_back(mP2->y());	
}

SkPointLine_distance_abstr::SkPointLine_distance_abstr(Geom3d::plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist):
	SkConstraint(baseplane_, { },
	{ abs((l->ptB()->x()-l->ptA()->x())*(p->y() - l->ptA()->y()) - (l->ptB()->y()-l->ptA()->y())*(p->x() - l->ptA()->x())) / sqrt(l->length2()) ^= dist })
{
	if(!p->fixed())
		mVars.insert(mVars.end(), { p->x(), p->y() });
	if(!l->fixed())
		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
}


SkPointLine_distance::SkPointLine_distance(Geom3d::plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist):
	SkPointLine_distance_abstr(baseplane_, p, l, dist),
	mPoint(p),
	mLine(l),
	mDistance(dist)
{

}

// SkCircleLine_distance::SkCircleLine_distance(Geom3d::plane_abstr* baseplane_, sketchCircle_ptr c, SkLineCurve* l, exp_ptr dist):
// 	SkPointLine_distance_abstr(baseplane_, c->center(), l, dist + c->radius())
// {

// }

SkLineLinePerpendicularity::SkLineLinePerpendicularity(Geom3d::plane_abstr* baseplane_, SkLineCurve* l1, SkLineCurve* l2):
	SkSprite_constraint<2>(baseplane_, { }, { }, "resources/textures/images/icons/sketch/constraints/perpendicularity.png"),
	mLine1(l1),
	mLine2(l2)
{
	if(!l1->fixed())
		mVars.insert(mVars.end(), { l1->ptA()->x(), l1->ptA()->y(), l1->ptB()->x(), l1->ptB()->y() });
	if(!l2->fixed())
		mVars.insert(mVars.end(), { l2->ptA()->x(), l2->ptA()->y(), l2->ptB()->x(), l2->ptB()->y() });

	exp_ptr x1 = (l1->ptA()->x()-l1->ptB()->x());
	exp_ptr y1 = (l1->ptA()->y()-l1->ptB()->y());
	exp_ptr x2 = (l2->ptA()->x()-l2->ptB()->x());
	exp_ptr y2 = (l2->ptA()->y()-l2->ptB()->y());

	mEqus.push_back(mod(acos(dot(x1, y1, x2, y2) / (sqrt(x1*x1+y1*y1) * sqrt(x2*x2+y2*y2))), 2.0*M_PI) ^= mod(ExpConst::pi2, 2.0*M_PI));

	// https://www.omnicalculator.com/math/angle-between-two-vectors

	mLine1->add_annot(annot(0));
	mLine2->add_annot(annot(1));
}

SkPointPoint_coincidence::SkPointPoint_coincidence(Geom3d::plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
	SkSprite_constraint<1>(baseplane_, { }, { p1->x() ^= p2->x(), p1->y() ^= p2->y()}, 
	"resources/textures/images/icons/sketch/constraints/coincidence.png"),
	mP1(p1),
	mP2(p2)
{
	if(!mP1->fixed())
		mVars.insert(mVars.end(), { mP1->x(), mP1->y() });
	if(!mP2->fixed())
		mVars.insert(mVars.end(), { mP2->x(), mP2->y() });
	
	mP1->add_annot(annot(0));
	mP2->add_annot(annot(0));
}
