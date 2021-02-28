
#include "coincidence_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

coincidence_tool::coincidence_tool(sketchDesign* env):
	constraint_tool(env)
{

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

void coincidence_tool::add_constraint()
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
		mEnv->target()->add_constraint(pointLine_distance::make(pt, std::static_pointer_cast<sketchLine>(curve), expConst::zero), curve);
	} else if(curve->type() == sketchEntity::CIRCLE) {
		mEnv->target()->add_constraint(pointCircle_distance::make(pt, std::static_pointer_cast<sketchCircle>(curve)), curve);
	} else {
		mEnv->target()->add_constraint(pointPoint_distance::make(pt, std::static_pointer_cast<sketchPoint>(curve), expConst::zero), mEntB);
	}
}
