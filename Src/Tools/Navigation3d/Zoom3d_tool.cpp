
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

#include "Zoom3d_tool.hpp"

#include <Workspaces/Workspace_abstr.hpp>
#include <Utils/Maths_util.hpp>
#include <Geometry/3d/Plane_abstr.hpp>

Zoom3d_tool::Zoom3d_tool(Workspace_abstr* env)
	: tool(env)
{
	// Attempt to load the cursor icon
	try {
		mCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Zoom.png", 30, 30), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void Zoom3d_tool::finish()
{
	is_zooming = false; // Reset
}
void Zoom3d_tool::init()
{
	is_zooming = false; // Reset
}

bool Zoom3d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) { // When left button is pressed
		glm::vec2 pos(event->x, -event->y);
		float delta = pos.y - prevPos.y; // Only the y movement is of interest
		if(is_zooming) {
			zoom(zoomStart, 0.02f * delta); // The movement is scaled arbitrarly, zoom around starting point
		} else {
			zoomStart = glm::vec2(event->x, event->y);
			is_zooming = true; // Now zooming, position are recorded
		}
		prevPos = pos; // Pos recorded
	} else {
		is_zooming = false;
	}
	return true;
}
bool Zoom3d_tool::manage_button_release(GdkEventButton* event)
{
	if(event->state & GDK_BUTTON1_MASK)
		finish();
	return true;
}
bool Zoom3d_tool::manage_scroll(GdkEventScroll* event)
{
	if(mEnv->state()) {
		zoom(glm::vec2(event->x, event->y), -0.015f * event->delta_y); // Delta is scaled arbitrarly, zooming around scroll origin
	}
	return true;
}

void Zoom3d_tool::zoom(glm::vec2 origin, float amount)
{
	// The goal of this function is to scale the model and translate it so that the scale origin appears fixed
	float scale = 1.0f + amount;
	Camera* cam = mEnv->state()->cam;
	
	glm::vec3 ray1 = cam->cast_ray(origin, false);
	glm::vec4 model_pos1 = cam->model() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Geom3d::Plane_abstr pl1(glm::vec3(model_pos1), cam->right(), cam->up());
	glm::vec3 inter1 = pl1.line_intersection(cam->pos(), ray1);
	cam->set_scale(cam->scale() * scale);
	cam->update(true);

	glm::vec3 ray2 = cam->cast_ray(origin, false);
	glm::vec4 model_pos2 = cam->model() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Geom3d::Plane_abstr pl2(glm::vec3(model_pos2), cam->right(), cam->up());
	glm::vec3 inter2 = pl2.line_intersection(cam->pos(), ray2);
	
	glm::vec3 trans_d = (inter1-inter2) * cam->fscale();
	cam->set_translation(cam->translation() - trans_d);
}