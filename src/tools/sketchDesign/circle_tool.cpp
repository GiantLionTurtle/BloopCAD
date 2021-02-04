
#include "circle_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/sketch.hpp>
#include <actions/common/addEntity_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

circle_tool::circle_tool(sketchDesign* env):
	tool(env)
{
	
}

void circle_tool::init()
{
	started = false; // Bring flag down
}

bool circle_tool::manage_mouse_move(GdkEventMotion* event)
{
	DEBUG_ASSERT(!mEnv->state(), LOG_ERROR("No valid state."));
	if(started) {
		camera_ptr cam = mEnv->state()->cam; // For ease of writing
		glm::vec2 circle_pos = mCircle->basePlane()->to_planePos(
            mCircle->basePlane()->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
        mCircle->set_radius(glm::length(circle_pos - mCircle->center_vec()));
	}
	return true;
}
bool circle_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic
	DEBUG_ASSERT(!mEnv->state(), LOG_ERROR("No valid state."));
	
	sketch_ptr target = mEnv->target();		
	DEBUG_ASSERT(!target, LOG_ERROR("No valid target."));

    // Find where the ray intersectpos_on_plane
    camera_ptr cam = mEnv->state()->cam; // For ease of writing
    geom_3d::plane_abstr_ptr pl = target->basePlane();
    glm::vec2 circle_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!started) {
		mEnv->state()->doc->make_glContext_current();
		mCircle = sketchCircle_ptr(new sketchCircle(circle_pos, 0.0f, pl));
		mEnv->state()->doc->push_action(std::make_shared<addEntity_action>(mCircle, target)); // Doc is passed to activate glContext
        started = true;
	} else {
        mCircle->set_radius(glm::length(circle_pos - mCircle->center_vec()));
        started = false;
	}
	return true;
}
