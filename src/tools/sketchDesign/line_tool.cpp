
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>
#include <entities/sketch.hpp>
#include <actions/sketchDesign/addLine_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

line_tool::line_tool(workspace* env):
	tool_abstract(env)
{
	
}

bool line_tool::manage_mouse_move(GdkEventMotion* event)
{
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);
		std::shared_ptr<camera> cam = mEnv->state()->cam;

		glm::vec2 screenPos(map((float)event->x, 0.0f, (float)mEnv->state()->doc->get_width(), 	-1.0f, 1.0f),
							map((float)event->y, (float)mEnv->state()->doc->get_height(), 0.0f, -1.0f, 1.0f));
		
		if(target) {
			glm::vec3 ray(
				(float)std::atan(map(screenPos.x, -1.0f, 1.0f, -(float)std::tan(glm::radians(10.0f)), (float)std::tan(glm::radians(10.0f))) * cam->aspectRatio().get()),
				(float)std::atan(map(screenPos.y, -1.0f, 1.0f, -(float)std::tan(glm::radians(10.0f)), (float)std::tan(glm::radians(10.0f)))),
				0.0f);
			ray.z = -std::sqrt(1.0f - (ray.x * ray.x + ray.y * ray.y));
			ray = cam->model_inv() * glm::vec4(ray, 0.0f);

			glm::vec3 pos_on_plane = target->basePlane()->line_intersection(mEnv->state()->cam->pos(), ray);
			if(!started) {
				startPos = pos_on_plane;
				started = true;
			} else {
				endPos = pos_on_plane;
				started = false;

				mEnv->state()->doc->push_action(std::shared_ptr<action>(new addLine_action(
					startPos, endPos, target, mEnv->state()->doc)));
			}
		}
	}
	return true;
}