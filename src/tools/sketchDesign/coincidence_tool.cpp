
#include "coincidence_tool.hpp"

#include <Drawables/sk/SkPoint.hpp>
#include <Drawables/sk/SkConstraint.hpp>
#include <actions/sketchDesign/toggleConstraint_action.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>


coincidence_tool::coincidence_tool(sketchDesign* env):
	constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/coincidence_cursor.png");
}

int coincidence_tool::could_add_entity(SkDrawable* ent)
{
	if(!ent) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_curve(mEntA) && is_point(ent)) {
			return add_states::WOULD_BE_COMPLETE;
		} else if(is_point(mEntA) && is_point_or_curve(ent)) {
			return add_states::WOULD_BE_COMPLETE;
		}
		return add_states::COULDNT_ADD;
	} else {
		return is_point_or_curve(ent) ? add_states::COULD_ADD : add_states::COULDNT_ADD;
	}
}

void coincidence_tool::create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent)
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
		// constr = pointLine_distance::make(pt, static_cast<sketchLine>(curve), expConst::zero);
		priority_ent = curve;
	} else if(is_curve(curve)) {
		// constr = pointCircle_distance::make(pt, static_cast<sketchCircle>(curve));
		priority_ent = curve;
	} else {
		// constr = pointPoint_coincidence::make(pt, static_cast<sketchPoint>(curve));
		constr = new pointPoint_coincidence(mEnv->target()->basePlane(), pt, static_cast<SkPoint*>(curve));
		priority_ent = mEntB;
	}
}
