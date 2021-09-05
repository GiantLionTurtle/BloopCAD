
#include "Perpendicularity_tool.hpp"

#include <Drawables/Sk/SkLine.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <document.hpp>

Perpendicularity_tool::Perpendicularity_tool(Sketch_ws* env):
	Constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/perpendicularity_cursor.png");
}

int Perpendicularity_tool::could_add_geom(SkDrawable* geom)
{
	if(!geom) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_line(geom))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_line(geom)) {
			return add_states::COULD_ADD;
		}
	}
	return add_states::COULDNT_ADD;
}
void Perpendicularity_tool::create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent)
{
	if(!mEntA || !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	constr = new SkLineLinePerpendicularity(mEnv->target()->basePlane(), 
		static_cast<SkLineCurve*>(mEntA), static_cast<SkLineCurve*>(mEntB));
	priority_ent = nullptr;
}