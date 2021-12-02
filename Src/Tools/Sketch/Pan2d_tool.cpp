
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

#include "Pan2d_tool.hpp"

#include <Utils/Expunge.hpp>
#include <Workspaces/Sketch_ws.hpp>
#include <Workspaces/Document.hpp>

Pan2d_tool::Pan2d_tool(Sketch_ws* env)
	: tool(env, Gdk::Cursor::create(Gdk::IRON_CROSS)) // Iron cross is the closest to the icon
	, mProxyCam(nullptr)
{
	// Attempt to load cursor icon
	try {
		mCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Pan.png", 24, 24), 12, 12);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}
Pan2d_tool::~Pan2d_tool()
{
	expunge(mProxyCam);
}

void Pan2d_tool::init()
{
	if(!mProxyCam)
		mProxyCam = new Camera();
}

bool Pan2d_tool::manage_button_press(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
    Camera* cam = mEnv->state()->cam;
	mProxyCam->copy(cam);
	mProxyCam->update();
	mDragStart = mEnv->target()->basePlane()->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false));
	mTranStart = cam->translation();
	is_moving = true;
	return true;
}
bool Pan2d_tool::manage_button_release(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
	is_moving = false;
	return true;
}
bool Pan2d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) {
		if(is_moving) {
			Camera* cam = mEnv->state()->cam;
			glm::vec3 pointedPos = mEnv->target()->basePlane()->line_intersection(mProxyCam->pos(), mProxyCam->cast_ray(glm::vec2(event->x, event->y), false));
			glm::vec3 mov = pointedPos - mDragStart;
			cam->set_translation(mTranStart + mov * mProxyCam->fscale()); // Move the model (no need to get fancy, it moves according to the "real position of the Camera")
		} else {
			is_moving = true; // Now moving, first point recorded
		}
	} else {
		is_moving = false; // Model no longer moving
	}
	return true;
}