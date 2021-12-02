
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#include "Coincidence_tool.hpp"

#include <Drawables/Sk/SkPoint.hpp>
#include <Drawables/Sk/SkLine.hpp>
#include <Drawables/Sk/SkCircle.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Workspaces/Document.hpp>


Coincidence_tool::Coincidence_tool(Sketch_ws* env)
	: Constraint_tool(env, CURVE | POINT | SKDRAWABLE)
{
	load_icon("Resources/Textures/Images/Icons/Sketch/Cursors/Coincidence_cursor.png");
}

int Coincidence_tool::could_add_geom(SkGeometry* geom)
{
	if(!geom) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_curve(mEntA) && is_point(geom)) {
			return add_states::WOULD_BE_COMPLETE;
		} else if(is_point(mEntA) && is_point_or_curve(geom)) {
			return add_states::WOULD_BE_COMPLETE;
		}
		return add_states::COULDNT_ADD;
	} else {
		return is_point_or_curve(geom) ? add_states::COULD_ADD : add_states::COULDNT_ADD;
	}
}

void Coincidence_tool::create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct)
{
	DEBUG_ASSERT(mEntA && mEntB, "Attempting to add incomplete constraint.");

	SkGeometry* curve;
	SkPoint* pt;
	if(is_point(mEntA)) {
		pt = static_cast<SkPoint*>(mEntA);
		curve = mEntB;
	} else {
		pt = static_cast<SkPoint*>(mEntB);
		curve = mEntA;
	}

	if(is_line(curve)) {
		// constr = new SkPointLine_coincidence(mEnv->target()->basePlane(), pt, static_cast<SkLineCurve*>(curve));
		priority_ent = curve;
	} else if(is_curve(curve)) {
		// constr = new SkPointCircle_coincidence(mEnv->target()->basePlane(), pt, static_cast<SkCircleCurve*>(curve));
		// constr = pointCircle_distance::make(pt, static_cast<sketchCircle>(curve));
		priority_ent = curve;
	} else {
		constr = new PointPoint_coincidence(static_cast<SkPoint*>(mEntA)->geom(), static_cast<SkPoint*>(mEntB)->geom());

		SkConstrAnnot* annot = nullptr;
		SkConstrAnnot::make_single_annot(annot, constr, mEnv->target()->basePlane(), 
		"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png");
		mEnv->target()->add_annot(annot);
		mEntA->add_annot(annot, true);
		mEntB->add_annot(annot, false);

		priority_ent = mEntB;
	}
}
