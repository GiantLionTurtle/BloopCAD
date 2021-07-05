
#include "horizontality_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchConstraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

horizontality_tool::horizontality_tool(sketchDesign* env):
	constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/horizontality_cursor.png");
}

int horizontality_tool::could_add_entity(sketchEntity_ptr ent)
{
	if(!ent) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_point(ent))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_point(ent)) {
			return add_states::COULD_ADD;
		} else if(is_line(ent)) {
			return add_states::WOULD_BE_COMPLETE;
		}
	}
	return add_states::COULDNT_ADD;
}

void horizontality_tool::add_constraint_impl(std::shared_ptr<constraint_entity>& constr, sketchEntity_ptr& priority_ent)
{
	if(!mEntA && !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	if(!mEntB) {
		constr = std::make_shared<line_horizontality>(mEnv->target()->basePlane(), std::static_pointer_cast<sketchLine>(mEntA));
	} else {
		constr = std::make_shared<pointPoint_horizontality>(mEnv->target()->basePlane(), std::static_pointer_cast<sketchPoint>(mEntA), std::static_pointer_cast<sketchPoint>(mEntB));
		priority_ent = mEntA;
	}
}