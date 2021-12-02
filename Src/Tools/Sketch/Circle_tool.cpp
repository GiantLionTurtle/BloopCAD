
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

#include "Circle_tool.hpp"

#include <Utils/Expunge.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Workspaces/Document.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>
#include <Utils/Maths_util.hpp>

Circle_tool::Circle_tool(Sketch_ws* env)
	: tool(env)
	, mCirclePreview(nullptr)
{
	DEBUG_ASSERT(mEnv, "No valid Workspace_abstr.");
}

void Circle_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mStarted = false; // Bring flag down
	mCirclePreview = nullptr;
}
void Circle_tool::finish()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mEnv->state()->doc->clear_toolPreview();
	expunge(mCirclePreview);
}

bool Circle_tool::manage_key_press(GdkEventKey* event)
{
	if(mStarted && event->keyval == GDK_KEY_Escape) {
		mStarted = false;
		mCirclePreview->set_exists(false);
		return false;
	}
	return true;
}

bool Circle_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(mStarted) {
		Camera* cam = mEnv->state()->cam; // For ease of writing
		glm::vec2 circle_pos = mCirclePreview->basePlane()->to_planePos(
            mCirclePreview->basePlane()->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
        mCirclePreview->set_radius(glm::length(circle_pos - mCirclePreview->center_pos()));
	}
	return true;
}
bool Circle_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	
	Sketch* target = mEnv->target();		
	DEBUG_ASSERT(target, "No valid target.");

    // Find where the ray intersectpos_on_plane
    Camera* cam = mEnv->state()->cam; // For ease of writing
    Geom3d::Plane_abstr* pl = target->basePlane();
    glm::vec2 circle_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!mStarted) {
		// mEnv->state()->doc->make_glContext_current();
		mCirclePreview = new SkCircle(circle_pos, 0.0f, target->basePlane(), false);
		mEnv->state()->doc->set_toolPreview(mCirclePreview);
        mStarted = true;
	} else {
		target->add_geometry(mCirclePreview);
		mEnv->state()->doc->push_action(std::make_shared<ToggleBaseObject_action>(mCirclePreview, true)); // Doc is passed to activate glContext
        mCirclePreview->set_radius(glm::length(circle_pos - mCirclePreview->center_pos()));
        mStarted = false;
		mCirclePreview = nullptr;
	}
	return true;
}
