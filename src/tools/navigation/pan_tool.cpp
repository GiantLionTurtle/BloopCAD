
#include "pan_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>

pan_tool::pan_tool(workspace* env): 
	tool_abstract(env, std::shared_ptr<compositeCursor>(new compositeCursor
		{Gdk::Cursor::create(Gdk::IRON_CROSS), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})) // Iron cross is the closest to the icon
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

bool pan_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov(pos.x-prevPos.x, pos.y-prevPos.y);
			float speed_ratio = (float)mEnv->state()->cam->zoom() * 0.002f; // Since the zoom of the camera isn't really retrievable yet, this is just an arbitrary factor * 1.0f
			std::shared_ptr<camera> cam = mEnv->state()->cam;
			cam->transformation().translation += (speed_ratio * abs_mov.x * cam->right() + speed_ratio * abs_mov.y * cam->up()); // Move the model (no need to get fancy, it moves according to the "real position of the camera")
		} else {
			is_moving = true; // Now moving, first point recorded
		}
		prevPos = pos; // Record point
	} else {
		is_moving = false; // Model no longer moving
	}
	return true;
}