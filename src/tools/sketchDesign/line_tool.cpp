
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/sketch.hpp>
#include <actions/common/addEntity_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

line_tool::line_tool(sketchDesign* env):
	tool(env)
{
	DEBUG_ASSERT(!mEnv, "No valid workspace.");
}

void line_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	started = false; // Bring flag down
}

bool line_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(started) {
		sketch_ptr target = mEnv->target();
		DEBUG_ASSERT(target, "No valid target.");

		camera_ptr cam = mEnv->state()->cam; // For ease of writing
		geom_3d::plane_abstr_ptr pl = target->basePlane();
		glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		mEndPos->set(line_pos);	
	}
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic	
	sketch_ptr target = mEnv->target();	
	DEBUG_ASSERT(target, "No valid target.");	

	// Find where the ray intersectpos_on_plane
	camera_ptr cam = mEnv->state()->cam; // For ease of writing
	geom_3d::plane_abstr_ptr pl = target->basePlane();
	glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!started) {
		mEnv->state()->doc->make_glContext_current();
		mStartPos = sketchPoint_ptr(new sketchPoint(line_pos, pl));
		mEndPos = sketchPoint_ptr(new sketchPoint(line_pos, pl));
		mLine = std::make_shared<sketchLine>(mStartPos, mEndPos, pl);
		started = true;

		mEnv->state()->doc->push_action(std::make_shared<addEntity_action>(mLine, target)); // Doc is passed to activate glContext
	} else {
		mEndPos->set(line_pos);
		started = false;
	}
	return true;
}
