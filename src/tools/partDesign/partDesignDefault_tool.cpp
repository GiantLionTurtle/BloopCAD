
#include "partDesignDefault_tool.hpp"

#include <entities/sketch.hpp>
#include <utils/errorLogger.hpp>
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

}

void partDesignDefault_tool::act_on_entity(entity_ptr ent)
{
	if(!mEnv) {
		LOG_WARNING("Null environnement in tool.");
		return;
	}

	sketch_ptr sk = std::dynamic_pointer_cast<sketch>(ent);
	if(sk) {		
		mEnv->state()->doc->push_action(std::shared_ptr<serial_action>(new serial_action({
			std::shared_ptr<action>(new enterSketchDesign_action(sk, true)),
			std::shared_ptr<action>(new quitPartDesign_action()),
			moveCamera_action::create_from_facingPlane(sk->basePlane(), 8.0, mEnv->state()->cam->state(), nullptr)
		})));
	}
}