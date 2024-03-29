
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "SkConstraint.hpp"

#include <ConstraintsSolver/Expression.hpp>
#include "SkPoint.hpp"
#include "SkLine.hpp"
#include "SkCircle.hpp"
#include <Drawables/Containers/Sketch.hpp>

#include <Utils/Debug_util.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <ConstraintsSolver/Constraint_abstr.hpp>

#include <array>
#include <string>

SkConstraint::SkConstraint(Geom3d::Plane_abstr* baseplane_, std::vector<var_ptr> vars, std::vector<equ_ptr> equs):
	SkIntDrawable(baseplane_),
	mVars(vars),
	mEqus(equs)
{
	set_name("constraint");
}

void SkConstraint::exists_impl(bool ex)
{
	// The variables are not toggled as other constraints might use them
	for(auto equ : mEqus)
		equ->set_exists(ex);
}

SkPointPoint_horizontality::SkPointPoint_horizontality(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
	SkSprite_constraint<2>(baseplane_, { }, { p1->y() ^= p2->y() },
	"Resources/Textures/Images/Icons/Sketch/Constraints/Horizontality"),
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

SkLine_horizontality::SkLine_horizontality(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l):
	SkSprite_constraint<1>(baseplane_, { }, { l->ptA()->y() ^= l->ptB()->y() },
	"Resources/Textures/Images/Icons/Sketch/Constraints/Horizontality"),
	mLine(l)
{
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->ptA()->y(), l->ptB()->y() });
	mLine->add_annot(annot(0));
}


SkPointPoint_verticality::SkPointPoint_verticality(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
	SkSprite_constraint<2>(baseplane_, { }, { p1->x() ^= p2->x() },
	"Resources/Textures/Images/Icons/Sketch/Constraints/Verticality.png"),
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

SkLine_verticality::SkLine_verticality(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l):
	SkSprite_constraint<1>(baseplane_, { }, { l->ptA()->x() ^= l->ptB()->x() },
	"Resources/Textures/Images/Icons/Sketch/Constraints/Verticality.png"),
	mLine(l)
{
	if(!mLine->fixed())
		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptB()->x() });
	mLine->add_annot(annot(0));
}

SkLine_length::SkLine_length(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l, exp_ptr length):
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

SkPointPoint_horizontalDistance::SkPointPoint_horizontalDistance(Geom3d::Plane_abstr* baseplane_, 
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

SkPointPoint_verticalDistance::SkPointPoint_verticalDistance(Geom3d::Plane_abstr* baseplane_, 
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

// SkPointLine_distance_abstr::SkPointLine_distance_abstr(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist):
// 	SkConstraint(baseplane_, { },
// 	{ abs((l->ptB()->x()-l->ptA()->x())*(p->y() - l->ptA()->y()) - (l->ptB()->y()-l->ptA()->y())*(p->x() - l->ptA()->x())) / sqrt(l->length2()) ^= dist })
// {
// 	if(!p->fixed())
// 		mVars.insert(mVars.end(), { p->x(), p->y() });
// 	if(!l->fixed())
// 		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
// }


// SkPointLine_distance::SkPointLine_distance(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist):
// 	SkPointLine_distance_abstr(baseplane_, p, l, dist),
// 	mPoint(p),
// 	mLine(l),
// 	mDistance(dist)
// {

// }



// SkCircleLine_distance::SkCircleLine_distance(Geom3d::Plane_abstr* baseplane_, sketchCircle_ptr c, SkLineCurve* l, exp_ptr dist):
// 	SkPointLine_distance_abstr(baseplane_, c->center(), l, dist + c->radius())
// {

// }

SkLineLinePerpendicularity::SkLineLinePerpendicularity(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l1, SkLineCurve* l2):
	SkSprite_constraint<2>(baseplane_, { }, { }, "Resources/Textures/Images/Icons/Sketch/Constraints/Perpendicularity.png"),
	mLine1(l1),
	mLine2(l2)
{
	if(!l1->fixed()) {}
		mVars.insert(mVars.end(), { l1->ptA()->x(), l1->ptA()->y(), l1->ptB()->x(), l1->ptB()->y() });
	if(!l2->fixed())
		mVars.insert(mVars.end(), { l2->ptA()->x(), l2->ptA()->y(), l2->ptB()->x(), l2->ptB()->y() });

	exp_ptr x1 = (l1->ptA()->x()-l1->ptB()->x());
	exp_ptr y1 = (l1->ptA()->y()-l1->ptB()->y());
	exp_ptr x2 = (l2->ptA()->x()-l2->ptB()->x());
	exp_ptr y2 = (l2->ptA()->y()-l2->ptB()->y());

	mEqus.push_back(mod(acos(dot(x1, y1, x2, y2) / (sqrt(x1*x1+y1*y1) * sqrt(x2*x2+y2*y2))), 2.0*M_PI) ^= ExpConst::pi2);

	// https://www.omnicalculator.com/math/angle-between-two-vectors

	mLine1->add_annot(annot(0));
	mLine2->add_annot(annot(1));
}

SkPointPoint_coincidence::SkPointPoint_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
	SkSprite_constraint<1>(baseplane_, { }, { p1->x() ^= p2->x(), p1->y() ^= p2->y()}, 
	"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png"),
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
SkPointLine_coincidence::SkPointLine_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l):
	SkSprite_constraint<1>(baseplane_, { },
	{ abs((l->ptB()->x()-l->ptA()->x())*(p->y() - l->ptA()->y()) - 
	(l->ptB()->y()-l->ptA()->y())*(p->x() - l->ptA()->x())) / sqrt(l->length2()) ^= ExpConst::zero }, 
	"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png"),	
	mPoint(p),
	mLine(l)
{
	if(!p->fixed())
		mVars.insert(mVars.end(), { p->x(), p->y() });
	if(!l->fixed())
		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
	
	mPoint->add_annot(annot(0));
}

SkPointCircle_coincidence::SkPointCircle_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkCircleCurve* c):
	SkSprite_constraint<1>(baseplane_, { },
	{ sqrt(pow(p->x() - c->center()->x(), 2) + pow(p->y() - c->center()->y(), 2)) ^= c->radius() }, 
	"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png"),	
	mPoint(p),
	mCircle(c)
{
	if(!p->fixed())
		mVars.insert(mVars.end(), { p->x(), p->y() });
	if(!c->fixed())
		mVars.insert(mVars.end(), { c->center()->x(), c->center()->y(), c->radius() });
	
	mPoint->add_annot(annot(0));
}