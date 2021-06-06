
#include "pan2d_tool.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <workspaces/sketchDesign.hpp>
#include <document.hpp>

pan2d_tool::pan2d_tool(sketchDesign* env):
	tool(env, compositeCursor_ptr(new compositeCursor
		{Gdk::Cursor::create(Gdk::IRON_CROSS), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})), // Iron cross is the closest to the icon
	mProxyCam(nullptr)
{
	// Attempt to load cursor icon
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/pan.png", 30, 30), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void pan2d_tool::init()
{
	if(!mProxyCam)
		mProxyCam = std::make_shared<camera>();
}

bool pan2d_tool::manage_button_press(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
    camera_ptr cam = mEnv->state()->cam;
	mProxyCam->copy(cam);
	mProxyCam->update();
	mDragStart = mEnv->target()->basePlane()->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false));
	mTranStart = cam->translation();
	is_moving = true;
	return true;
}
bool pan2d_tool::manage_button_release(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
	is_moving = false;
	return true;
}
bool pan2d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) {
		if(is_moving) {
			camera_ptr cam = mEnv->state()->cam;
			glm::vec3 pointedPos = mEnv->target()->basePlane()->line_intersection(mProxyCam->pos(), mProxyCam->cast_ray(glm::vec2(event->x, event->y), false));
			glm::vec3 mov = pointedPos - mDragStart;
			cam->set_translation(mTranStart + mov); // Move the model (no need to get fancy, it moves according to the "real position of the camera")
		} else {
			is_moving = true; // Now moving, first point recorded
		}
	} else {
		is_moving = false; // Model no longer moving
	}
	return true;
}