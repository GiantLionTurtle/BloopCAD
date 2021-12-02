
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Pan3d_tool.hpp"

#include <Workspaces/Workspace_abstr.hpp>

Pan3d_tool::Pan3d_tool(Workspace_abstr* env)
	: tool(env, Gdk::Cursor::create(Gdk::IRON_CROSS)) // Iron cross is the closest to the icon
{
	// Attempt to load cursor icon
	try {
		mCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Pan.png", 30, 30), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

bool Pan3d_tool::manage_button_press(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
	prevPos = glm::vec2(event->x, -event->y);
	is_moving = true;
	return true;
}
bool Pan3d_tool::manage_button_release(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
	is_moving = false;
	return true;
}
bool Pan3d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov(pos.x-prevPos.x, pos.y-prevPos.y);
			float speed_ratio = (float)mEnv->state()->cam->zoom() * 0.002f; // Since the zoom of the Camera isn't really retrievable yet, this is just an arbitrary factor * 1.0f
			Camera* cam = mEnv->state()->cam;
			cam->internalPos() -= glm::vec3(abs_mov * speed_ratio, 0.0f);
		} else {
			is_moving = true; // Now moving, first point recorded
		}
		prevPos = pos; // Record point
	} else {
		is_moving = false; // Model no longer moving
	}
	return true;
}