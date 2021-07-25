
#include "sketchDesignDefault_tool.hpp"

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

sketchDesignDefault_tool::sketchDesignDefault_tool(sketchDesign* env):
	tool<sketchDesign>(env),
	// mDraggedEnt(nullptr),
	mHoveredEnt(nullptr),
	mSelectionRect(nullptr),
	mAllowedToMove(false)
{
	mSelectionRect = new SkSelRect(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), nullptr);
}
sketchDesignDefault_tool::~sketchDesignDefault_tool()
{
	delete mSelectionRect;
}

void sketchDesignDefault_tool::init()
{
	mSelectionRect->set_basePlane(mEnv->target()->basePlane());
	mAllowedToMove = false;
	mMoving = false;
}

bool sketchDesignDefault_tool::manage_key_press(GdkEventKey* event)
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

bool sketchDesignDefault_tool::manage_button_press(GdkEventButton* event)
{
	Camera_ptr cam = mEnv->state()->cam;
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	mAllowedToMove = false;
	if(event->button == 1) {
		auto clickedPoint = mEnv->target()->closest(screenPos, cam.get(), cam->cast_ray(screenPos), DRAWABLE);
		if(clickedPoint.ent) {
			mAllowedToMove = true;
			if(!clickedPoint.ent->selected()) {
				if(!(event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK))
					mEnv->target()->unselect_all();
			}
		} else {
			mEnv->target()->unselect_all();

			geom_3d::plane_abstr_ptr pl = mEnv->target()->basePlane();
			glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(screenPos)));
			mSelectionRect->set_points(pos, pos);
			mEnv->state()->doc->set_toolPreview(mSelectionRect);
			mStartPos = screenPos;
		}
	}
	return true;
}
bool sketchDesignDefault_tool::manage_button_release(GdkEventButton* event)
{
	if(event->button == 1) {
		mEnv->state()->doc->clear_toolPreview();
	}

	// mDraggedEnt = nullptr;
	
	if(mMoving) {
		int i = 0;
		mEnv->state()->doc->push_action(std::make_shared<applySnapshot_action>(mEnv->target()->deltaSnapshot(mStartSnapshot), true));
		LOG_WARNING("Implement snapshot with variables");
	}

	mAllowedToMove = false;
	mMoving = false;
	return true;
}

bool sketchDesignDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	Sketch* sk = mEnv->target();
	DEBUG_ASSERT(sk, "No valid sketch.");

	geom_3d::plane_abstr_ptr pl = sk->basePlane();
	Camera_ptr cam = mEnv->state()->cam;
	glm::vec2 mousePos(event->x, event->y);
	glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(mousePos, false)));

	if(event->state & GDK_BUTTON1_MASK) {
		if(mAllowedToMove) {
			if(!mMoving) {
				mMoving = true;
				LOG_WARNING("Implement move snapshots");
				mStartSnapshot = mEnv->target()->snapshot();
			}
			mEnv->target()->move_selected(mPrevPos, pos, mPrevMousePos - mousePos);
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
		auto hovered = mEnv->target()->closest(screenPos, cam.get(), cam->cast_ray(screenPos), DRAWABLE);
		if(hovered.ent != mHoveredEnt) {
			if(mHoveredEnt) {
				mHoveredEnt->set_hover(false);
			}
			if(hovered.ent) {
				hovered.ent->set_hover(true);
			}
			mHoveredEnt = hovered.ent;
		}
	}
	mPrevPos = pos;
	mPrevMousePos = mousePos;

	return true;
}
