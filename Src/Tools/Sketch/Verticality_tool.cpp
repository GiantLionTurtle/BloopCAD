
#include "Verticality_tool.hpp"

#include <Drawables/Sk/SkLine.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Workspaces/Document.hpp>

Verticality_tool::Verticality_tool(Sketch_ws* env):
	Constraint_tool(env)
{
	load_icon("Resources/Textures/Images/Icons/Sketch/Cursors/Verticality_cursor.png");
}

int Verticality_tool::could_add_geom(SkDrawable* geom)
{
	if(!geom) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_point(geom))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_point(geom)) {
			return add_states::COULD_ADD;
		} else if(is_line(geom)) {
			return add_states::WOULD_BE_COMPLETE;
		}
	}
	return add_states::COULDNT_ADD;
}

void Verticality_tool::create_constraint(Constraint_abstr*& constr, SkDrawable*& priority_ent)
{
	if(!mEntA && !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	if(!mEntB) {
		// constr = new SkLine_verticality(mEnv->target()->basePlane(), static_cast<SkLineCurve*>(mEntA));
	} else {
		// constr = new SkPointPoint_verticality(mEnv->target()->basePlane(), static_cast<SkPoint*>(mEntA), static_cast<SkPoint*>(mEntB));
		priority_ent = mEntB;
	}
}