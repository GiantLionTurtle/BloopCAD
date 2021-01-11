
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>
#include <entities/tangibleEntities/line.hpp>
#include <entities/sketch.hpp>
#include <actions/common/addEntity_action.hpp>
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
		sketch_ptr target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
		if(!target) {
			LOG_WARNING("No valid target.");
			return true;
		}
		camera_ptr cam = mEnv->state()->cam; // For ease of writing
		plane_abstract_ptr pl = target->basePlane();
		glm::vec2 line_pos = pl->point_3d_to_2d(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		mEndPos->set_pos(line_pos);	
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
	
	sketch_ptr target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);		
	if(!target) {
		LOG_WARNING("No valid target.");
		return true;
	}

	// Find where the ray intersectpos_on_plane
	camera_ptr cam = mEnv->state()->cam; // For ease of writing
	plane_abstract_ptr pl = target->basePlane();
	glm::vec2 line_pos = pl->point_3d_to_2d(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!started) {
		mEnv->state()->doc->make_glContext_current();
		mStartPos = sketchPoint_ptr(new sketchPoint(line_pos, pl));
		mEndPos = sketchPoint_ptr(new sketchPoint(line_pos, pl));
		mLine = std::make_shared<line>(line_abstract(mStartPos, mEndPos));
		started = true;

		mEnv->state()->doc->push_action(std::make_shared<addEntity_action>(mLine, target)); // Doc is passed to activate glContext
	} else {
		mEndPos->set_pos(line_pos);
		started = false;
	}
	return true;
}
