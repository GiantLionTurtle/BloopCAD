
#include "point_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <Drawables/Sketch.hpp>
#include <actions/common/toggleBaseObject_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

point_tool::point_tool(sketchDesign* env):
	tool(env)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void point_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
}

bool point_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic
	Sketch* target = mEnv->target();	
	DEBUG_ASSERT(target, "No valid target.");

	// Find where the ray intersectpos_on_plane
	Camera_ptr cam = mEnv->state()->cam; // For ease of writing
	geom_3d::plane_abstr_ptr pl = target->basePlane();
	glm::vec2 point_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	// mEnv->state()->doc->make_glContext_current();
	mCurrentPoint = sketchPoint_ptr(new sketchPoint(point_pos, pl));
	target->add_geometry(mCurrentPoint);
	mEnv->state()->doc->push_action(std::make_shared<toggleBaseObject_action>(mCurrentPoint, true));
	return true;
}
