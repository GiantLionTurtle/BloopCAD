
#include "partDesignDefault_tool.hpp"

#include <utils/DebugUtils.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>
#include <actions/sketchDesign/enterSketchDesign_action.hpp>
#include <actions/partDesign/quitPartDesign_action.hpp>
#include <actions/common/serial_action.hpp>

#include <iostream>

partDesignDefault_tool::partDesignDefault_tool(partDesign* env):
	simpleSelector_tool(env)
{
	if(!mEnv) {
		LOG_ERROR("Null environnement in tool.");
	}
}

void partDesignDefault_tool::act_on_entity(entity_ptr ent)
{
	ent->invoke_workspace(mEnv->state()->doc);
}