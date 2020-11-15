
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

				glm::vec4 planePos = target->transform() * glm::vec4(sketchPlane->origin(), 1.0f);
				glm::vec4 v = target->transform() * target->transform() * glm::vec4(sketchPlane->v(), 1.0f);
				glm::vec4 w = target->transform() * target->transform() * glm::vec4(sketchPlane->w(), 1.0f);
				glm::vec3 campos = glm::vec3(planePos.x, planePos.y, planePos.z) + 
				glm::normalize(glm::cross(glm::vec3(v.x, v.y, v.z), glm::vec3(w.x, w.y, w.z))) * 1.8f; // TODO: make this 1.8f thingy less arbitrary
				newState->cam->set_target(mEnv->state()->cam->target());
				newState->cam->set_cartesian(mEnv->state()->cam->pos_cartesian());
				newState->cam->set_targetState({ true, campos, planePos }, 2000);
			}
			std::cout<<"Start sketch!\n";
		}
	}
	return true;
}