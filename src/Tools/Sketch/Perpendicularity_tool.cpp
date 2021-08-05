
#include "Perpendicularity_tool.hpp"

#include <Drawables/sk/SkLine.hpp>
#include <actions/sketchDesign/toggleConstraint_action.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

Perpendicularity_tool::Perpendicularity_tool(sketchDesign* env):
	Constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/perpendicularity_cursor.png");
}

int Perpendicularity_tool::could_add_entity(SkDrawable* ent)
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
void Perpendicularity_tool::create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent)
{
	if(!mEntA || !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}
	LOG_WARNING("nfksjnfew");
	constr = new SkLineLinePerpendicularity(mEnv->target()->basePlane(), 
		static_cast<SkLineCurve*>(mEntA), static_cast<SkLineCurve*>(mEntB));
	priority_ent = nullptr;
}