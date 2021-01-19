
#include "circle_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/sketch.hpp>
#include <actions/common/addEntity_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

circle_tool::circle_tool(workspace* env):
	tool_abstract(env)
{
	
}

void circle_tool::init()
{
	started = false; // Bring flag down
}

bool circle_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(!mEnv->state()) {
		LOG_WARNING("No valid state.");
		return true;
	}
	if(started) {
		camera_ptr cam = mEnv->state()->cam; // For ease of writing
		glm::vec2 circle_pos = mCircle->basePlane()->point_3d_to_2d(
            mCircle->basePlane()->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
        mCircle->set_radius(glm::length(circle_pos - mCircle->center_val()));
	}
	return true;
}
bool circle_tool::manage_button_press(GdkEventButton* event)
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
    glm::vec2 circle_pos = pl->point_3d_to_2d(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!started) {

		mEnv->state()->doc->make_glContext_current();
		mCircle = sketchCircle_ptr(new sketchCircle(circle_abstract(circle_pos, 0.0f), pl));
		mEnv->state()->doc->push_action(std::make_shared<addEntity_action>(mCircle, target)); // Doc is passed to activate glContext
        started = true;
	} else {
        mCircle->set_radius(glm::length(circle_pos - mCircle->center_val()));
        started = false;
	}
	return true;
}
