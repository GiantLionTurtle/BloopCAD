
#include "perpendicularity_tool.hpp"

#include <entities/geometry/point_abstract.hpp>
#include <entities/geometry/line_abstract.hpp>
#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

perpendicularity_tool::perpendicularity_tool(workspace* env):
	simpleSelector_tool(env)
{
    mFilter = [](entity_ptr ent) -> bool { 
		return 	std::dynamic_pointer_cast<line_abstract>(ent).operator bool(); 
	};
}

void perpendicularity_tool::init()
{
	mStarted = false;

	if(!mEnv->state()) {
		LOG_WARNING("No valid state.");
		return;
	}
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
	if(!mEnv) {
		LOG_WARNING("No valid workspace.");
		return true;
	}
	
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
	sketch_ptr sk = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);
	if(!sk) {
		LOG_WARNING("No valid sketch.");
		return;
	}
    if(!mSysA.equations || !mSysB.equations) {
        LOG_WARNING("Invalid equations.");
        return;
    }
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
			}
		}
	}
	mSysA.set_tmpConstant(false);
	mSysA.set_tmpConstant(false);
}