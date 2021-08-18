
#include "SketchDefault_tool.hpp"

#include <document.hpp>
#include <Drawables/Sketch.hpp>
#include <utils/DebugUtils.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>
#include <actions/common/toggleBaseObject_action.hpp>
#include <actions/common/serial_action.hpp>
#include <actions/sketchDesign/applySnapshot_action.hpp>

#include <iostream>

SketchDefault_tool::SketchDefault_tool(sketchDesign* env):
	tool<sketchDesign>(env),
	// mDraggedEnt(nullptr),
	mHoveredEnt(nullptr),
	mSelectionRect(nullptr),
	mAllowedToMove(false)
{
	mSelectionRect = new SkSelRect(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), nullptr);
}
SketchDefault_tool::~SketchDefault_tool()
{
	delete mSelectionRect;
}

void SketchDefault_tool::init()
{
	mSelectionRect->set_basePlane(mEnv->target()->basePlane());
	mAllowedToMove = false;
	mMoving = false;
}

bool SketchDefault_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_Escape) {
		mEnv->target()->unselect_all();
		return false;
	} else if(event->keyval == GDK_KEY_Delete) {
		auto deleteAllSelected = mEnv->target()->delete_selected();

		mEnv->state()->doc->push_action(deleteAllSelected);
		mEnv->target()->unselect_all();
	}
	return true;
}

bool SketchDefault_tool::manage_button_press(GdkEventButton* event)
{
	Camera_ptr cam = mEnv->state()->cam;
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	mAllowedToMove = false;
	if(event->button == 1) {
		auto clickedPoint = mEnv->target()->closest_draggable(screenPos, cam.get(), DRAWABLE);
		auto hovered = clickedPoint ? clickedPoint.ent : nullptr;
		if(hovered) {
			mAllowedToMove = true;
			int n_sel = mEnv->target()->n_selected();
			if(n_sel == 0 || (n_sel == 1 && hovered->selected())) {
					// mEnv->target()->dragConstr()->set_equality(clickedPoint.pt);
			} else {
				// mEnv->target()->dragConstr()->clear(); // TODO: Check if useful
			}

			if(!hovered->selected()) {
				if(!(event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK))
					mEnv->target()->unselect_all();
				hovered->select();
			}
		} else {
			mEnv->target()->unselect_all();

			Geom3d::Plane_abstr* pl = mEnv->target()->basePlane();
			glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(screenPos)));
			mSelectionRect->set_points(pos, pos);
			mEnv->state()->doc->set_toolPreview(mSelectionRect);
			mStartPos = screenPos;
		}
	}
	return true;
}
bool SketchDefault_tool::manage_button_release(GdkEventButton* event)
{
	if(event->button == 1) {
		mEnv->state()->doc->clear_toolPreview();
	}
	
	if(mMoving) {
		int i = 0;
		mEnv->state()->doc->push_action(std::make_shared<applySnapshot_action>(mEnv->target()->deltaSnapshot(mStartSnapshot), true));
		LOG_WARNING("Implement snapshot with variables");
	}

	mEnv->target()->dragConstr()->clear();
	mAllowedToMove = false;
	mMoving = false;
	return true;
}

bool SketchDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	Sketch* sk = mEnv->target();
	DEBUG_ASSERT(sk, "No valid sketch.");

	Geom3d::Plane_abstr* pl = sk->basePlane();
	Camera_ptr cam = mEnv->state()->cam;
	glm::vec2 mousePos(event->x, event->y);
	glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(mousePos, false)));

	if(event->state & GDK_BUTTON1_MASK) {
		if(mAllowedToMove) {
			if(!mMoving) {
				mMoving = true;
				mStartSnapshot = mEnv->target()->snapshot();
			}
			// if(!mEnv->target()->dragConstr()->valid()) {
				mEnv->target()->move_selected(mPrevPos, pos, mPrevMousePos - mousePos);
			// } else {
				// mEnv->target()->dragConstr()->dragTo(pos);
			// }
			bool update_attempt = sk->update_constraints(true, false);
		} else {
			mSelectionRect->set_endPoint(pos);
			if(event->x > mStartPos.x) {
				mSelectionRect->set_mode(SkSelRect::COVER);
			} else {
				mSelectionRect->set_mode(SkSelRect::TOUCH);
			}
			glm::vec2 a = mSelectionRect->start(), b = mSelectionRect->end(); // Just so that it is more concise
			glm::vec2 bottom_right(std::max(a.x, b.x), std::min(a.y, b.y)), top_left(std::min(a.x, b.x), std::max(a.y, b.y));
			sk->select_within(top_left, bottom_right, mSelectionRect->mode());
		}
	} else {
		Camera_ptr cam = mEnv->state()->cam;
		glm::vec2 screenPos = glm::vec2(event->x, event->y);
		auto dragPt = mEnv->target()->closest_draggable(screenPos, cam.get(), DRAWABLE);
		auto hovered = dragPt ? dragPt.ent : nullptr;
		if(hovered != mHoveredEnt) {
			if(mHoveredEnt) {
				mHoveredEnt->set_hover(false);
			}
			if(hovered) {
				hovered->set_hover(true);
			}
			mHoveredEnt = hovered;
		}
	}
	mPrevPos = pos;
	mPrevMousePos = mousePos;

	return true;
}
