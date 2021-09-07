
#include "Point_tool.hpp"

#include <document.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>

Point_tool::Point_tool(Sketch_ws* env):
	tool(env)
{
	DEBUG_ASSERT(mEnv, "No valid Workspace_abstr.");
}

void Point_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
}

bool Point_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic
	Sketch* target = mEnv->target();	
	DEBUG_ASSERT(target, "No valid target.");

	// Find where the ray intersectpos_on_plane
	Camera* cam = mEnv->state()->cam; // For ease of writing
	Geom3d::Plane_abstr* pl = target->basePlane();
	glm::vec2 point_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	// mEnv->state()->doc->make_glContext_current();
	mCurrentPoint = new SkPoint(point_pos, pl, false);
	target->add_geometry(mCurrentPoint);
	mEnv->state()->doc->push_action(std::make_shared<ToggleBaseObject_action>(mCurrentPoint, true));
	return true;
}
