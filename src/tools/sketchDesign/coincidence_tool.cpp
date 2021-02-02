
#include "coincidence_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

coincidence_tool::coincidence_tool(workspace* env):
	simpleSelector_tool(env)
{
    mFilter = [](entity_ptr ent) -> bool { 
		return 	std::dynamic_pointer_cast<geom_2d::point_abstr>(ent).operator bool() ||
				std::dynamic_pointer_cast<geom_2d::line_abstr>(ent).operator bool(); 
	};
}

void coincidence_tool::init()
{
	mStarted = false;

	if(!mEnv->state()) {
		LOG_WARNING("No valid state.");
		return;
	}
	// Check if there is only one item in the document's selection stack and if it is a plane, use it
	if(mEnv->state()->doc->selection_size() > 0 && mFilter(mEnv->state()->doc->selection_at(0).ent)) {
		mSysA = mEnv->state()->doc->selection_at(0).ent->coincidence();
		mStarted = true;
		if(mEnv->state()->doc->selection_size() > 1 && mFilter(mEnv->state()->doc->selection_at(0).ent)) {
			mSysB = mEnv->state()->doc->selection_at(1).ent->coincidence();
			mStarted = false;
			add_constraint();
		}
		
	}
}

bool coincidence_tool::manage_button_press(GdkEventButton* event)
{
	if(!mEnv) {
		LOG_WARNING("No valid workspace.");
		return true;
	}
	
	entity_ptr ent = entity_at_point(glm::vec2(event->x, event->y));
	if(!ent) {
		return true;
	}
	if(!mStarted) {
		mSysA = ent->coincidence();
		mStarted = true;
	} else {
		mSysB = ent->coincidence();
		mStarted = false;

		add_constraint();
	}
	return true;
}

void coincidence_tool::add_constraint()
{
	sketch_ptr sk = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);
	if(!sk) {
		LOG_WARNING("No valid sketch.");
		return;
	}

	sk->backup_system();
	// Try to move only one point at a time
	mSysA.set_tmpConstant(true);
	if(!sk->add_constraint(mSysA - mSysB)) {
		mSysA.set_tmpConstant(false);
		mSysA.set_tmpConstant(true);
		if(!sk->update_constraints()) {
			mSysA.set_tmpConstant(false);
			if(!sk->update_constraints()) {
				sk->revert_system_to_backup();
				LOG_WARNING("Could not solve system.");
			}
		}
	}
	mSysA.set_tmpConstant(false);
	mSysA.set_tmpConstant(false);
}