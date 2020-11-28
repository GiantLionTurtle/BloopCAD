
#include "orbit_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

orbit_tool::orbit_tool(workspace* env):
	tool_abstract(env, std::shared_ptr<compositeCursor>(new compositeCursor
		{Gdk::Cursor::create(Gdk::CROSSHAIR), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})), 
	is_moving(false),
	tmp_right(0.0f, 0.0f, 0.0f)
{
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
	is_moving = false;
}

bool orbit_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_q) {
		tmp_right = mEnv->state()->cam->right();
		std::cout<<"Saving right vector: "<<glm::to_string(tmp_right)<<"\n";
	}
	return true;
}

bool orbit_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && (event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK)) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov = (pos-prevPos) * 0.005f;
			
			std::shared_ptr<camera> cam = mEnv->state()->cam;
			cam->orientation() += glm::vec3(
				-abs_mov.y, 
				(cam->flipped() ? -abs_mov.x : abs_mov.x), 
				0.0f);
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
} 
