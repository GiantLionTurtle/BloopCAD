
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

#include "Line_tool.hpp"

#include <Utils/Expunge.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Workspaces/Document.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Actions/Common/Parallel_action.hpp>
#include <Tools/Sketch/Coincidence_tool.hpp>

Line_tool::Line_tool(Sketch_ws* env)
	: Tool(env)
	, mEndPos(nullptr)
	, mLinePreview(nullptr)
	, mLastAdded(nullptr)
{
	DEBUG_ASSERT(mEnv, "No valid Workspace_abstr.");
}

void Line_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mLastAdded = nullptr;
	mLinePreview = nullptr;
	mStarted = false; // Bring flag down
	mEndPos = nullptr;
}
void Line_tool::finish()
{
	mEnv->state()->doc->clear_toolPreview();
	expunge(mLinePreview); // Should not be non-nullptr, but it might change eventually..
	mLastAdded = nullptr;
}

bool Line_tool::manage_key_press(GdkEventKey* event)
{
	if(mStarted && event->keyval == GDK_KEY_Escape) {
		mStarted = false;
		mLinePreview->set_exists(false);
		mEndPos = nullptr;
		return false;	
	}
	return true;
}

bool Line_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(mStarted) {
		Sketch* target = mEnv->target();
		DEBUG_ASSERT(target, "No valid target.");

		Camera* cam = mEnv->state()->cam; // For ease of writing
		Geom3d::Plane_abstr* pl = target->basePlane();
		glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		mLinePreview->ptB()->set(line_pos);
	}
	return true;
}
bool Line_tool::manage_button_press(GdkEventButton* event)
{
	// Find where the ray intersectpos_on_plane
	Camera* cam = mEnv->state()->cam; // For ease of writing
	Geom3d::Plane_abstr* pl = mEnv->target()->basePlane();
	glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	add_point(line_pos);
	return true;
}

SkPoint* Line_tool::add_point(glm::vec2 pt)
{
	if(!mStarted) {
		mLinePreview = new SkLine(pt, pt, mEnv->target()->basePlane(), false);
		mEnv->state()->doc->set_toolPreview(mLinePreview);
		mStarted = true;
		return mLinePreview->ptA();
	} else {

		std::shared_ptr<ToggleConstraint_action> hingeConstraintAction = nullptr;
		Constraint* constr = nullptr;
		if(mEndPos) {
			mEnv->coincidence()->add_geom(mEndPos);
			mEnv->coincidence()->add_geom(mLinePreview->ptA());
			SkGeometry* trash = nullptr;
			Action_ptr annotAct = nullptr;
			mEnv->coincidence()->create_constraint(constr, trash, annotAct);
			
			mEnv->coincidence()->clear_geometries();
			hingeConstraintAction = std::make_shared<ToggleConstraint_action>(mEnv->target(), constr, true, true);
		}

		mLinePreview->ptB()->set(pt);
		mEnv->target()->add_geometry(mLinePreview);
		mLinePreview->release();

		auto lineAction = std::make_shared<ToggleBaseObject_action>(mLinePreview, true);
		Action_ptr compoundAction;
		if(hingeConstraintAction) {
			mEnv->target()->add_constraint(constr, nullptr);
			compoundAction = std::shared_ptr<Parallel_action>(new Parallel_action({
				lineAction,
				hingeConstraintAction
				}));
		} else {
			compoundAction = lineAction;
		}
		mEnv->state()->doc->push_action(compoundAction);
		mEnv->state()->doc->clear_toolPreview();

		mEndPos = mLinePreview->ptB();
		mLastAdded = mLinePreview;
		mLinePreview = nullptr;

		mLinePreview = new SkLine(pt, pt, mEnv->target()->basePlane(), false);
		mEnv->state()->doc->set_toolPreview(mLinePreview);
		return mEndPos;
	}
}
