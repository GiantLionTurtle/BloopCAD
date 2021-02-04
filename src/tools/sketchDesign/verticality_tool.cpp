
#include "verticality_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

verticality_tool::verticality_tool(sketchDesign* env):
	simpleSelector_tool(env)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");

	mFilter = [](entity_ptr ent) -> bool { 
		return 	std::dynamic_pointer_cast<geom_2d::point_abstr>(ent).operator bool() ||
				std::dynamic_pointer_cast<geom_2d::line_abstr>(ent).operator bool(); 
	};
}

void verticality_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");

	// Check if there is only one item in the document's selection stack and if it is a plane, use it
	if(mEnv->state()->doc->selection_size() > 0 && mFilter(mEnv->state()->doc->selection_at(0).ent)) {
		if(!set_systems(mEnv->state()->doc->selection_at(0).ent->verticality()) && 
		mEnv->state()->doc->selection_size() > 1 && mFilter(mEnv->state()->doc->selection_at(0).ent)) {
			set_systems(mEnv->state()->doc->selection_at(1).ent->verticality());
		}
		
	}
}

bool verticality_tool::manage_button_press(GdkEventButton* event)
{
	entity_ptr ent = entity_at_point(glm::vec2(event->x, event->y));
	if(!ent) {
		return true;
	}
	set_systems(ent->verticality());
	return true;
}

bool verticality_tool::set_systems(std::vector<subEquationsSystem> sys)
{
	if(!mStarted && sys.size() == 1) {
		mSysA = sys[0];
		mStarted = true;
		return false;
	} else if(mStarted && sys.size() == 1) {
		mSysB = sys[0];
		mStarted = false;
		add_constraint();
		return true;
	} else if(!mStarted && sys.size() == 2) {
		mSysA = sys[0];
		mSysB = sys[1];
		add_constraint();
		return true;
	}
	return false;
}

void verticality_tool::add_constraint()
{
	sketch_ptr sk = mEnv->target();
	DEBUG_ASSERT(sk, "No valid sketch.");

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