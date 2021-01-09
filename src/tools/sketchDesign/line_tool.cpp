
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>
#include <entities/tangibleEntities/line.hpp>
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
	if(!mEnv->state()) {
		LOG_WARNING("No valid state.");
		return true;
	}
	if(started) {
		std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
		if(!target) {
			LOG_WARNING("No valid target.");
			return true;
		}
		endPos->set_pos(pos_on_plane(target->basePlane(), glm::vec2(event->x, event->y)));
	}
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic

	if(!mEnv->state()) {
		LOG_WARNING("No valid state.");
		return true;
	}
	std::shared_ptr<camera> cam = mEnv->state()->cam; // For ease of writing
	
	std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
	if(!target) {
		LOG_WARNING("No valid target.");
		return true;
	}
	// Find where the ray intersectpos_on_plane
	glm::vec2 line_pos = pos_on_plane(target->basePlane(), glm::vec2(event->x, event->y));

	if(!started) {
		mEnv->state()->doc->make_glContext_current();
		startPos = std::shared_ptr<sketchPoint>(new sketchPoint(target->basePlane(), line_pos));
		endPos = std::shared_ptr<sketchPoint>(new sketchPoint(target->basePlane(), line_pos));
		started = true;
		mEnv->state()->doc->push_action(std::shared_ptr<action>(new addLine_action(
			startPos, endPos, target, mEnv->state()->doc))); // Doc is passed to activate glContext
	} else {
		endPos->set_pos(line_pos);
		started = false;
	}
	return true;
}

glm::vec2 line_tool::pos_on_plane(std::shared_ptr<plane_abstract> plane_, glm::vec2 mousePos)
{
	std::shared_ptr<camera> cam = mEnv->state()->cam; // For ease of writing
	
	float screen_dist = 1.0f / std::tan(cam->FOV() / 2.0f);
	float half_screenWidth = cam->aspectRatio();
	glm::vec3 pos_on_screen(map((float)mousePos.x, 0.0f, (float)mEnv->state()->doc->get_width(), -half_screenWidth, half_screenWidth),
							map((float)mousePos.y, (float)mEnv->state()->doc->get_height(), 0.0f, -1.0f, 1.0f), 
							-screen_dist);
	glm::vec3 ray = cam->model_inv() * glm::vec4(glm::normalize(pos_on_screen), 0.0f);
	glm::vec3 inter = plane_->line_intersection(cam->pos(), ray);
	return plane_->point_3d_to_2d(inter);
}