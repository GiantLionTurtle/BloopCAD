
#include "coincidence_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <constraintsSolver/constraint.hpp>
#include <actions/sketchDesign/toggleConstraint_action.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

coincidence_tool::coincidence_tool(sketchDesign* env):
	constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/coincidence_cursor.png");
}

int coincidence_tool::could_add_entity(sketchEntity_ptr ent)
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

void coincidence_tool::add_constraint_impl(std::shared_ptr<constraint_abstract>& constr, sketchEntity_ptr& priority_ent)
{
	if(!mEntA || !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	sketchEntity_ptr curve;
	sketchPoint_ptr pt;
	if(is_point(mEntA)) {
		pt = std::static_pointer_cast<sketchPoint>(mEntA);
		curve = mEntB;
	} else {
		pt = std::static_pointer_cast<sketchPoint>(mEntB);
		curve = mEntA;
	}

	if(curve->type() == sketchEntity::LINE) {
		constr = pointLine_distance::make(pt, std::static_pointer_cast<sketchLine>(curve), expConst::zero);
		priority_ent = curve;
	} else if(curve->type() == sketchEntity::CIRCLE) {
		constr = pointCircle_distance::make(pt, std::static_pointer_cast<sketchCircle>(curve));
		priority_ent = curve;
	} else {
		constr = pointPoint_coincidence::make(pt, std::static_pointer_cast<sketchPoint>(curve));
		priority_ent = mEntB;
	}
}
