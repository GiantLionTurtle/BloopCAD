
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

void line_tool::init()
{
	started = false; // Bring flag down
}

bool line_tool::manage_mouse_move(GdkEventMotion* event)
{
	// Not implemented yet ;-;
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic

	if(mEnv->state()) {
		std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
		if(target) {
			std::shared_ptr<camera> cam = mEnv->state()->cam; // For ease of writing

			// Create a ray from the "real" camera position with the mouse position and the FOV
			float tan_half_FOV = std::tan(cam->FOV() / 2.0f);
			glm::vec3 ray(
				(float)std::atan(map((float)event->x, 0.0f, (float)mEnv->state()->doc->get_width(), -tan_half_FOV, tan_half_FOV)) * cam->aspectRatio().get(),
				(float)std::atan(map((float)event->y, (float)mEnv->state()->doc->get_height(), 0.0f, -tan_half_FOV, tan_half_FOV)),
				0.0f);
			ray.z = -std::sqrt(1.0f - (ray.x * ray.x + ray.y * ray.y));

			// Transform the ray in the world
			ray = cam->model_inv() * glm::vec4(ray, 0.0f);

			// Find where the ray intersect
			glm::vec3 pos_on_plane = target->basePlane()->line_intersection(mEnv->state()->cam->pos(), ray);

			// Somewhat trivial line logic
			if(!started) {
				startPos = pos_on_plane;
				started = true;
			} else {
				endPos = pos_on_plane;
				started = false;

				mEnv->state()->doc->push_action(std::shared_ptr<action>(new addLine_action(
					startPos, endPos, target, mEnv->state()->doc))); // Doc is passed to activate glContext
			}
		}
	}
	return true;
}