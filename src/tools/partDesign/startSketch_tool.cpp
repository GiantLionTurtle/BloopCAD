
#include "startSketch_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>

#include <entities/part.hpp>

#include <actions/partDesign/createSketch_action.hpp>
#include <actions/switchWorkspace_action.hpp>

startSketch_tool::startSketch_tool(workspace* env):
	planeSelector_tool(env)
{

}

bool startSketch_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		std::shared_ptr<plane_abstract> sketchPlane = std::dynamic_pointer_cast<plane_abstract>(entity_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y)));

		if(sketchPlane) {
			std::shared_ptr<part> target = std::dynamic_pointer_cast<part>(mEnv->state()->target);
			if(target) {
				mEnv->state()->doc->push_action(std::shared_ptr<action>(new createSketch_action(sketchPlane, target)));
				mEnv->state()->doc->push_action(std::shared_ptr<action>(new switchWorkspace_action(mEnv->state()->doc, "sketchDesign")));
				std::shared_ptr<workspaceState> newState = mEnv->state()->doc->currentWorkspaceState();

				newState->cam->set(mEnv->state()->cam);
				
				LOG_WARNING("Startsketch disabled.");
				// newState->cam->set_target(mEnv->state()->cam->target());
				// newState->cam->set_cartesian(mEnv->state()->cam->pos_cartesian());
				// newState->cam->set_targetState({ true, campos, planePos }, 2000);
			}
			std::cout<<"Start sketch!\n";
		}
	}
	return true;
}