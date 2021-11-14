
#include "Orbit3d_tool.hpp"

#include <Workspaces/Workspace_abstr.hpp>

#include <glm/glm.hpp>

Orbit3d_tool::Orbit3d_tool(Workspace_abstr* env)
	: tool(env, Gdk::Cursor::create(Gdk::CROSSHAIR)) // CROSSHAIR is the closest cursor if it can't load the icon
	, is_moving(false)
{
	// Load the cursor icon
	try {
		mCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Orbit.png", 34, 34), 17, 17);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void Orbit3d_tool::init() 
{ 
	is_moving = false; // Reset
}
void Orbit3d_tool::finish()
{
	is_moving = false; // Reset
}

bool Orbit3d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && (event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK)) { // Left button or middle mouse
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov = (pos-prevPos) * 0.005f; // Arbitrary scaling of the movement
			Camera* cam = mEnv->state()->cam; // Convenience temporary
			cam->set_orientation(cam->orientation() + glm::vec3(
				-abs_mov.y, 
				(cam->flipped() ? -abs_mov.x : abs_mov.x), // When flipped, the rotation around the y axis is flipped for usage consistency
				0.0f));
		} else {
			is_moving = true; // Moving begun, there is one recorded point
		}
		prevPos = pos; // Record pos
	} else {
		is_moving = false; // The mouse might be moving, but not the Camera
	}
	return true;
} 
bool Orbit3d_tool::manage_button_release(GdkEventButton* event)
{ 
	if(event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK) // Finish the move on button release
		finish(); 
	return true; 
}
