
#include "orbit_tool.hpp"

#include <utils/preferences.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

#include <glm/glm.hpp>

orbit_tool::orbit_tool(workspace* env):
	tool_abstract(env, std::shared_ptr<compositeCursor>(new compositeCursor
		{Gdk::Cursor::create(Gdk::CROSSHAIR), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})), // CROSSHAIR is the closest cursor if it can't load the icon
	is_moving(false)
{
	// Load the cursor icon
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/orbit.png", 50, 50), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void orbit_tool::init() 
{ 
	is_moving = false; // Reset
}
void orbit_tool::finish()
{
	is_moving = false; // Reset
}

bool orbit_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && (event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK)) { // Left button or middle mouse
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov = (pos-prevPos) * 0.005f; // Arbitrary scaling of the movement
			camera_ptr cam = mEnv->state()->cam; // Convenience temporary
			cam->set_orientation(cam->orientation() + glm::vec3(
				-abs_mov.y, 
				(cam->flipped() ? -abs_mov.x : abs_mov.x), // When flipped, the rotation around the y axis is flipped for usage consistency
				0.0f));
		} else {
			is_moving = true; // Moving begun, there is one recorded point
		}
		prevPos = pos; // Record pos
	} else {
		is_moving = false; // The mouse might be moving, but not the camera
	}
	return true;
} 
bool orbit_tool::manage_button_release(GdkEventButton* event)
{ 
	if(event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK) // Finish the move on button release
		finish(); 
	return true; 
}
