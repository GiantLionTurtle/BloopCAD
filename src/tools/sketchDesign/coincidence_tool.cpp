
#include "coincidence_tool.hpp"

#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

coincidence_tool::coincidence_tool(workspace* env):
	simpleSelector_tool(env)
{
    mFilter = [](entity_ptr ent) -> bool { return std::dynamic_pointer_cast<point_abstract>(ent).operator bool(); };
}

void coincidence_tool::init()
{
	mStarted = false;

	if(!mEnv->state()) {
		LOG_WARNING("No valid state.");
		return;
	}
	// Check if there is only one item in the document's selection stack and if it is a plane, use it
	if(mEnv->state()->doc->selection_size() > 0 && (mPointA = std::dynamic_pointer_cast<point_abstract>(mEnv->state()->doc->selection_at(0).ent))) {
		mStarted = true;
		if(mEnv->state()->doc->selection_size() > 1 && (mPointB = std::dynamic_pointer_cast<point_abstract>(mEnv->state()->doc->selection_at(1).ent))) {
			mStarted = false;
			point_point_coincidence();
		}
		
	}
}

bool coincidence_tool::manage_button_press(GdkEventButton* event)
{
	if(!mEnv) {
		LOG_WARNING("No valid workspace.");
		return true;
	}
	
	point_abstract_ptr pt = std::dynamic_pointer_cast<point_abstract>(entity_at_point(glm::vec2(event->x, event->y)));
	if(pt) {
		if(!mStarted) {
			mPointA = pt;
			mStarted = true;
		} else {
			mPointB = pt;
			mStarted = false;

			point_point_coincidence();
		}
	}
	return true;
}

void coincidence_tool::point_point_coincidence()
{
	sketch_ptr sk = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);
	if(!sk) {
		LOG_WARNING("No valid sketch.");
		return;
	}

	sk->backup_system();
	// Try to move only one point at a time
	mPointB->set_tmpConstant(true);
	if(!sk->add_constraint(std::make_shared<coincidence_constraint>(mPointA, mPointB))) {
		mPointB->set_tmpConstant(false);
		mPointA->set_tmpConstant(true);
		if(!sk->update_constraints()) {
			mPointA->set_tmpConstant(false);
			if(!sk->update_constraints()) {
				sk->revert_system_to_backup();
			}
		}
	}
	mPointB->set_tmpConstant(false);
	mPointA->set_tmpConstant(false);
}