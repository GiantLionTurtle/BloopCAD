
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
		
		float screen_dist = 1.0f / std::tan(cam->FOV() / 2.0f);
		float half_screenWidth = cam->aspectRatio().get();
		glm::vec3 pos_on_screen(map((float)mousePos.x, 0.0f, (float)mEnv->state()->doc->get_width(), -half_screenWidth, half_screenWidth),
								map((float)mousePos.y, (float)mEnv->state()->doc->get_height(), 0.0f, -1.0f, 1.0f), 
								-screen_dist);
		glm::vec3 ray = cam->model_inv() * glm::vec4(glm::normalize(pos_on_screen), 0.0f);
		return target->basePlane()->line_intersection(mEnv->state()->cam->pos(), ray);
	}
	LOG_WARNING("No target.");
	return glm::vec3(0, 0, 0);
}