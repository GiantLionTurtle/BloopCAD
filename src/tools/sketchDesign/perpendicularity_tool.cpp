
#include "perpendicularity_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <constraintsSolver/constraint.hpp>
#include <actions/sketchDesign/toggleConstraint_action.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

perpendicularity_tool::perpendicularity_tool(sketchDesign* env):
	constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/perpendicularity_cursor.png");
}

int perpendicularity_tool::could_add_entity(sketchEntity_ptr ent)
{
	if(!ent) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_line(ent))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_line(ent)) {
			return add_states::COULD_ADD;
		}
	}
	return add_states::COULDNT_ADD;
}
void perpendicularity_tool::add_constraint_impl(std::shared_ptr<constraint_entity>& constr, sketchEntity_ptr& priority_ent)
{
	if(!mEntA || !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}
	LOG_WARNING("nfksjnfew");
	constr = std::make_shared<lineLinePerpendicularity>(mEnv->target()->basePlane(), 
		std::static_pointer_cast<sketchLine>(mEntA), std::static_pointer_cast<sketchLine>(mEntB));
	priority_ent = nullptr;
}