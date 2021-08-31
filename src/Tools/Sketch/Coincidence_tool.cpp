
#include "Coincidence_tool.hpp"

#include <Drawables/Sk/SkPoint.hpp>
#include <Drawables/Sk/SkConstraint.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <document.hpp>


Coincidence_tool::Coincidence_tool(sketchDesign* env):
	Constraint_tool(env, CURVE | POINT | SKDRAWABLE)
{
	load_icon("resources/textures/images/icons/sketch/cursors/coincidence_cursor.png");
}

int Coincidence_tool::could_add_geom(SkDrawable* geom)
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

void Coincidence_tool::create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent)
{
	if(!mEntA || !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	SkDrawable* curve;
	SkPoint* pt;
	if(is_point(mEntA)) {
		pt = static_cast<SkPoint*>(mEntA);
		curve = mEntB;
	} else {
		pt = static_cast<SkPoint*>(mEntB);
		curve = mEntA;
	}

	if(is_line(curve)) {
		// constr = SkPointLine_distance::make(pt, static_cast<sketchLine>(curve), ExpConst::zero);
		priority_ent = curve;
	} else if(is_curve(curve)) {
		// constr = pointCircle_distance::make(pt, static_cast<sketchCircle>(curve));
		priority_ent = curve;
	} else {
		constr = new SkPointPoint_coincidence(mEnv->target()->basePlane(), pt, static_cast<SkPoint*>(curve));
		priority_ent = mEntB;
	}
}
