
#include "perpendicularity_tool.hpp"

#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>
#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

perpendicularity_tool::perpendicularity_tool(sketchDesign* env):
	simpleSelector_tool(env)
{
	DEBUG_ASSERT(!mEnv, LOG_ERROR("No valid workspace."));

    mFilter = [](entity_ptr ent) -> bool { 
		return 	std::dynamic_pointer_cast<geom_2d::line_abstr>(ent).operator bool(); 
	};
}

void perpendicularity_tool::init()
{
	mStarted = false;

	DEBUG_ASSERT(!mEnv->state(), LOG_ERROR("No valid state."));

	// Check if there is only one item in the document's selection stack and if it is a plane, use it
	if(mEnv->state()->doc->selection_size() > 0 && mFilter(mEnv->state()->doc->selection_at(0).ent)) {
		mSysA = mEnv->state()->doc->selection_at(0).ent->direction();
		mStarted = true;
		if(mEnv->state()->doc->selection_size() > 1 && mFilter(mEnv->state()->doc->selection_at(0).ent)) {
			mSysB = mEnv->state()->doc->selection_at(1).ent->direction();
			mStarted = false;
			add_constraint();
		}
		
	}
}

bool perpendicularity_tool::manage_button_press(GdkEventButton* event)
{	
	entity_ptr ent = entity_at_point(glm::vec2(event->x, event->y));
	if(!ent) {
		return true;
	}
	if(!mStarted) {
		mSysA = ent->direction();
		mStarted = true;
	} else {
		mSysB = ent->direction();
		mStarted = false;

		add_constraint();
	}
	return true;
}

void perpendicularity_tool::add_constraint()
{
	sketch_ptr sk = mEnv->target();
	DEBUG_ASSERT(!sk, LOG_ERROR("No valid sketch."));
	DEBUG_ASSERT(!mSysA.equations || !mSysB.equations, LOG_ERROR("No valid equation."));

	sk->backup_system();	
    std::vector<variable_ptr> vars;
    vars.insert(vars.end(), mSysA.variables.begin(), mSysA.variables.end());
    vars.insert(vars.end(), mSysB.variables.begin(), mSysB.variables.end());

	mSysA.set_tmpConstant(true);
	if(!sk->add_constraint({ vars, { acos(dot(mSysA.equations, mSysB.equations) / (length(mSysA.equations) * length(mSysB.equations))) - expConst::pi2 } })) {
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