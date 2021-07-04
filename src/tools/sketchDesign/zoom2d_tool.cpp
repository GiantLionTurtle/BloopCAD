
#include "zoom2d_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <utils/mathUtils.hpp>
#include <geometry/geometry_3d/plane_abstr.hpp>

zoom2d_tool::zoom2d_tool(sketchDesign* env): 
	tool(env)
{
	// Attempt to load the cursor icon
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/zoom.png", 30, 30), 10, 10);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void zoom2d_tool::finish()
{
	mIsZooming = false; // Reset
}
void zoom2d_tool::init()
{
	mIsZooming = false; // Reset
}
bool zoom2d_tool::manage_button_press(GdkEventButton* event)
{
	if(event->button == 1) {
		mIsZooming = true;
		Camera_ptr cam = mEnv->state()->cam;
		geom_3d::plane_abstr_ptr pl = mEnv->target()->basePlane();
		glm::vec2 screen_pos(event->x, event->y);
		mZoomStart_world = pl->line_intersection(cam->pos(), cam->cast_ray(screen_pos));
	}
	return true;
}
bool zoom2d_tool::manage_button_release(GdkEventButton* event)
{
	if(event->button == 1)
		finish();
	return true;
}
bool zoom2d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	glm::vec2 pos(event->x, -event->y);
	if(mIsZooming) {
		float delta = pos.y - mPrevPos.y; // Only the y movement is of interest
		zoom(mZoomStart_world, 0.02f * delta); // The movement is scaled arbitrarly, zoom around starting point
	}
	mPrevPos = pos; // Pos recorded
	return true;
}

bool zoom2d_tool::manage_scroll(GdkEventScroll* event)
{
	Camera_ptr cam = mEnv->state()->cam;
	geom_3d::plane_abstr_ptr pl = mEnv->target()->basePlane();
	glm::vec2 screen_pos(event->x, event->y);
	zoom(pl->line_intersection(cam->pos(), cam->cast_ray(screen_pos)), -0.015f * event->delta_y); // Delta is scaled arbitrarly, zooming around scroll origin
	return true;
}

void zoom2d_tool::zoom(glm::vec3 world_origin, float amount)
{
	// The goal of this function is to scale the model and translate it so that the scale origin appears fixed
	Camera_ptr cam = mEnv->state()->cam;
	float scale = 1.0f + amount;
	float delta_scale = cam->fscale() * scale - cam->fscale();
	
	cam->set_scale(cam->scale() * scale);
	cam->set_translation(cam->translation() - world_origin * delta_scale);
}