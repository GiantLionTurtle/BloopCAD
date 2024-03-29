
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#include "Zoom2d_tool.hpp"

#include <Workspaces/Document.hpp>
#include <Geometry/3d/Plane_abstr.hpp>

Zoom2d_tool::Zoom2d_tool(Sketch_ws* env): 
	tool(env)
{
	// Attempt to load the cursor icon
	try {
		mCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Zoom.png", 30, 30), 10, 10);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void Zoom2d_tool::finish()
{
	mIsZooming = false; // Reset
}
void Zoom2d_tool::init()
{
	mIsZooming = false; // Reset
}
bool Zoom2d_tool::manage_button_press(GdkEventButton* event)
{
	if(event->button == 1) {
		mIsZooming = true;
		Camera* cam = mEnv->state()->cam;
		Geom3d::Plane_abstr* pl = mEnv->target()->basePlane();
		glm::vec2 screen_pos(event->x, event->y);
		mZoomStart_world = pl->line_intersection(cam->pos(), cam->cast_ray(screen_pos));
	}
	return true;
}
bool Zoom2d_tool::manage_button_release(GdkEventButton* event)
{
	if(event->button == 1)
		finish();
	return true;
}
bool Zoom2d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	glm::vec2 pos(event->x, -event->y);
	if(mIsZooming) {
		float delta = pos.y - mPrevPos.y; // Only the y movement is of interest
		zoom(mZoomStart_world, 0.02f * delta); // The movement is scaled arbitrarly, zoom around starting point
	}
	mPrevPos = pos; // Pos recorded
	return true;
}

bool Zoom2d_tool::manage_scroll(GdkEventScroll* event)
{
	Camera* cam = mEnv->state()->cam;
	Geom3d::Plane_abstr* pl = mEnv->target()->basePlane();
	glm::vec2 screen_pos(event->x, event->y);
	zoom(pl->line_intersection(cam->pos(), cam->cast_ray(screen_pos)), -0.015f * event->delta_y); // Delta is scaled arbitrarly, zooming around scroll origin
	return true;
}

void Zoom2d_tool::zoom(glm::vec3 world_origin, float amount)
{
	// The goal of this function is to scale the model and translate it so that the scale origin appears fixed
	Camera* cam = mEnv->state()->cam;
	float scale = 1.0f + amount;
	float delta_scale = cam->fscale() * scale - cam->fscale();
	
	cam->set_scale(cam->scale() * scale);
	cam->set_translation(cam->translation() - world_origin * delta_scale);
}