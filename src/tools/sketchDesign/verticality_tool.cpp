
#include "verticality_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

verticality_tool::verticality_tool(sketchDesign* env):
	constraint_tool(env)
{

}

int verticality_tool::could_add_entity(sketchEntity_ptr ent)
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

void verticality_tool::add_constraint()
{
	if(!mEntA && !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	if(!mEntB) {
		mEnv->target()->add_constraint(pointPoint_verticality::make(std::static_pointer_cast<sketchLine>(mEntA)));
	} else {
		mEnv->target()->add_constraint(pointPoint_verticality::make(std::static_pointer_cast<sketchPoint>(mEntA), std::static_pointer_cast<sketchPoint>(mEntB)));
	}
}