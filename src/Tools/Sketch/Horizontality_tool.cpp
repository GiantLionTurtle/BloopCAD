
#include "Horizontality_tool.hpp"

#include <Drawables/sk/SkLine.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

Horizontality_tool::Horizontality_tool(sketchDesign* env):
	Constraint_tool(env)
{
	load_icon("resources/textures/images/icons/sketch/cursors/horizontality_cursor.png");
}

int Horizontality_tool::could_add_entity(SkDrawable* ent)
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

void Horizontality_tool::create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent)
{
	if(!mEntA && !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	if(!mEntB) {
		constr = new SkLine_horizontality(mEnv->target()->basePlane(), static_cast<SkLineCurve*>(mEntA));
	} else {
		constr = new SkPointPoint_horizontality(mEnv->target()->basePlane(), static_cast<SkPoint*>(mEntA), static_cast<SkPoint*>(mEntB));
		priority_ent = mEntA;
	}
}