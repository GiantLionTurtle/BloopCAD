
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
	mMode(modes::NORMAL),
	mSelArea(nullptr),
	mCurrentHover(nullptr)
{
	mSelArea = new SkSelRect(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), nullptr);
}
SketchDefault_tool::~SketchDefault_tool()
{
	delete mSelArea;
}

void SketchDefault_tool::init()
{
	mSelArea->set_basePlane(mEnv->target()->basePlane());
	mMode = modes::NORMAL;
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
	if(event->type == GDK_DOUBLE_BUTTON_PRESS) // Double click are not handled yet
		return true;

	if(event->button != 1) // Not a rule or anything but it is the only button used currently
		return true;

	// Can't enter this function without being in NORMAL mode, debug check
	DEBUG_ASSERT(mMode == modes::NORMAL, "Unexpected use mode, mode is " + std::to_string(mMode));

	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	glm::vec2 plPos = mEnv->state()->cam->screen_to_plane(screenPos, mEnv->target()->basePlane());

	update_dragCandidate(screenPos);
	
	if(mDragCandidate.ent) {
		mLastPlPos = plPos;
		mDragCandidate.ent->select();
	} else {
		mEnv->target()->unselect_all();
		mSelArea->set_points(plPos, plPos);
		mEnv->state()->doc->set_toolPreview(mSelArea);
		mAreaSelStartX = event->x;
		mMode = modes::AREASELECT;
	}
	return true;
}

bool SketchDefault_tool::manage_button_release(GdkEventButton* event)
{
	if(event->button != 1) // Not a rule or anything but it is the only button used currently
		return true;
	update_dragCandidate(glm::vec2(event->x, event->y));

	if(mMode == modes::NORMAL && !mDragCandidate.ent) {
		mEnv->target()->unselect_all(); // Clear selection if the user clicks an empty space
	} else if(mMode == modes::DRAGGING) {
		mEnv->state()->doc->push_action(std::make_shared<applySnapshot_action>(mEnv->target()->deltaSnapshot(mStartSnapshot), true));
		mEnv->target()->dragConstr()->clear();
	} else if(mMode == modes::AREASELECT) {
		mEnv->state()->doc->clear_toolPreview(); // Clear the selection rectangle
	}
	mMode = modes::NORMAL;
	return true;
}

bool SketchDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	Sketch* sk = mEnv->target();
	glm::vec2 cursorPos(event->x, event->y);
	glm::vec2 plPos = mEnv->state()->cam->screen_to_plane(cursorPos, mEnv->target()->basePlane());
	update_hover(cursorPos);

	if(mMode == modes::NORMAL && event->state & GDK_BUTTON1_MASK && mDragCandidate.ent) {
		mStartSnapshot = sk->snapshot();
		mMode = modes::DRAGGING;
	}

	if(mMode == modes::DRAGGING) {
		// Move drawables
		// if(sk->n_selected() == 1) {
			// mDragCandidate.ent
		// } else {
			sk->move_selected(plPos - mLastPlPos);
		// }
		mLastPlPos = plPos;
		sk->update_constraints(true, false);
	} else if(mMode == modes::AREASELECT) {
		areaSelect(plPos, cursorPos.x);
	}

	return true;
}

void SketchDefault_tool::update_dragCandidate(glm::vec2 cursorPos)
{
	Camera* cam = mEnv->state()->cam.get();
	Sketch* sk = mEnv->target();
	if(cursorPos != mLastCursorPos && !sk->need_update() && !cam->need_update()) { // Prevent recomputing the same thing multiple times
		mDragCandidate = sk->closest_draggable(cursorPos, cam, DRAWABLE);
		mLastCursorPos = cursorPos;
	}
}
void SketchDefault_tool::update_hover(glm::vec2 cursorPos)
{
	Drawable* hovered_drw = mEnv->target()->closest(cursorPos, mEnv->state()->cam.get(), DRAWABLE).ent;
	if(hovered_drw != mCurrentHover) {
		if(mCurrentHover) {
			mCurrentHover->set_hover(false);
		}
		if(hovered_drw) {
			hovered_drw->set_hover(true);
		}
		mCurrentHover = hovered_drw;
	}
}
void SketchDefault_tool::areaSelect(glm::vec2 plPos, float cursorX)
{
	Sketch* sk = mEnv->target();
	mSelArea->set_endPoint(plPos);
	if(cursorX > mAreaSelStartX) {
		mSelArea->set_mode(SkSelRect::COVER);
	} else {
		mSelArea->set_mode(SkSelRect::TOUCH);
	}
	glm::vec2 a = mSelArea->start(), b = mSelArea->end(); // Just so that it is more concise
	glm::vec2 bottom_right(std::max(a.x, b.x), std::min(a.y, b.y)), top_left(std::min(a.x, b.x), std::max(a.y, b.y));
	sk->select_within(top_left, bottom_right, mSelArea->mode());
}