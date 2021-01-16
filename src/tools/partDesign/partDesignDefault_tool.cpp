
#include "partDesignDefault_tool.hpp"

#include <entities/sketch.hpp>
#include <utils/errorLogger.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>

#include <iostream>

partDesignDefault_tool::partDesignDefault_tool(workspace* env):
	simpleSelector_tool(env)
{

}

void partDesignDefault_tool::act_on_entity(entity_ptr ent)
{
	if(!mEnv) {
		LOG_WARNING("Null environnement in tool.");
		return;
	}

	sketch_ptr sk = std::dynamic_pointer_cast<sketch>(ent);
	if(sk) {
		mEnv->state()->doc->push_action(std::shared_ptr<action>(new switchWorkspace_action("sketchDesign")));
		workspaceState_ptr newState = mEnv->state()->doc->currentWorkspaceState(); // This tool is still owned by partDesign so it has to retrieve the sketchDesign workspace state
		newState->cam->set(mEnv->state()->cam);
		mEnv->state()->doc->push_action(moveCamera_action::create_from_facingPlane(sk->basePlane(), 8.0, newState->cam));
		sk->origin()->show();
	}
}