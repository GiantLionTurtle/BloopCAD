
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>
#include <entities/line.hpp>
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
	if(mEnv->state() && started) {
		std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
		if(target) {
			std::shared_ptr<line> currLine = std::dynamic_pointer_cast<line>(target->get_last());
			if(currLine) {
				currLine->set_pointB(pos_on_plane(glm::vec2(event->x, event->y)));
				mEnv->state()->doc->make_glContext_current();
				currLine->update_VB();
			}
		}
	}
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic

	if(mEnv->state()) {
		std::shared_ptr<camera> cam = mEnv->state()->cam; // For ease of writing
		
		std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
		if(target) {
			// Find where the ray intersect
			glm::vec3 pos_on_plane_ = pos_on_plane(glm::vec2(event->x, event->y));

			// Somewhat trivial line logic
			if(!started) {
				startPos = pos_on_plane_;
				started = true;
				mEnv->state()->doc->push_action(std::shared_ptr<action>(new addLine_action(
					startPos, startPos, target, mEnv->state()->doc))); // Doc is passed to activate glContext
			} else {
				endPos = pos_on_plane_;
				started = false;
			}
		}
	}
	return true;
}

glm::vec3 line_tool::pos_on_plane(glm::vec2 mousePos)
{
	std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
	if(target) {
		std::shared_ptr<camera> cam = mEnv->state()->cam; // For ease of writing
		
		float cos_half_FOVy = std::cos(M_PI/2 - cam->FOV() / 2.0f);
		float half_FOVx = std::atan(std::tan(cam->FOV()/2.0f)*cam->aspectRatio().get());
		float cos_half_FOVx = std::cos(M_PI/2 - half_FOVx);
		float heightx = sin(M_PI/2-half_FOVx);
		float heighty = sin(M_PI/2-cam->FOV() / 2.0f);
		float mouse_cosx = map((float)mousePos.x, 0.0f, (float)mEnv->state()->doc->get_width(), -cos_half_FOVx, cos_half_FOVx);
		float mouse_cosy = map((float)mousePos.y, (float)mEnv->state()->doc->get_height(), 0.0f, -cos_half_FOVy, cos_half_FOVy);

		glm::vec3 angles(
			M_PI/2.0f-std::atan2(heightx, mouse_cosx),
			std::atan2(heighty, mouse_cosy),
			0.0f);
		// ray.z = -std::sqrt(1.0f - (ray.x * ray.x + ray.y * ray.y));
		glm::vec3 ray(0.0f, 0.0f, -1.0f);
		ray = glm::rotate(glm::angleAxis(angles.y, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(-angles.x, glm::vec3(0.0f, 1.0f, 0.0f)), ray);
		
		// Transform the ray in the world
		// ray = cam->model_inv() * glm::vec4(ray, 1.0f);
		std::cout<<"ray: "<<glm::to_string(ray)<<"\n";//<<" /w "<<std::atan(height / map((float)mousePos.x, 0.0f, (float)mEnv->state()->doc->get_width(), -cos_half_FOVx, cos_half_FOVx))<<"\n";
		// Find where the ray intersect
		return target->basePlane()->line_intersection(mEnv->state()->cam->pos(), ray);
	}
	LOG_WARNING("No target.");
	return glm::vec3(0, 0, 0);
}