
#include "SketchDefault_tool.hpp"

#include <Utils/Expunge.hpp>
#include <Workspaces/Document.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Utils/Debug_util.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Workspaces/Document.hpp>
#include <Actions/Common/MoveCamera_action.hpp>
#include <Actions/Common/SwitchWorkspace_action.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>
#include <Actions/Common/Serial_action.hpp>
#include <Actions/Sketch/ApplySnapshot_action.hpp>

#include <iostream>

SketchDefault_tool::SketchDefault_tool(Sketch_ws* env):
	tool<Sketch_ws>(env),
	mMode(modes::NORMAL),
	mSelArea(nullptr),
	mCurrentHover(nullptr)
{
	mSelArea = new SkSelRect(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), nullptr);
}
SketchDefault_tool::~SketchDefault_tool()
{
	expunge(mSelArea);
}

void SketchDefault_tool::init()
{
	mSelArea->set_basePlane(mEnv->target()->basePlane());
	mMode = modes::NORMAL;
}

bool SketchDefault_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_e || event->keyval == GDK_KEY_E) {
		mEnv->target()->toggle_solve();
		
		verbose(VERBOSE_INNERSTEPS, "Toggled solver to "<<std::boolalpha<<mEnv->target()->solve_allowed());
	}
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
		if(!(event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK) && !mDragCandidate.ent->selected())
			mEnv->target()->unselect_all();
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
		mEnv->state()->doc->push_action(std::make_shared<ApplySnapshot_action>(mEnv->target(), mEnv->target()->deltaSnapshot(mStartSnapshot), true));
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


	if(mMode == modes::DRAGGING) {
		// Move drawables
		if(sk->n_selected() == 1 && mDragCandidate.ent) {
			mDragCandidate.ent->move_selected(plPos - mLastPlPos);
			mDragCandidate.ent->release();
			sk->dragConstr()->dragTo(plPos);
			sk->update_constraints(false, true);
		} else {
			sk->move_selected(plPos - mLastPlPos);
		}
	} else if(mMode == modes::AREASELECT) {
		areaSelect(plPos, cursorPos.x);
	} else if(mMode == modes::NORMAL && event->state & GDK_BUTTON1_MASK && mDragCandidate.ent) {
		mStartSnapshot = sk->snapshot();
		if(sk->n_selected() == 1) {
			sk->dragConstr()->set_equality(mDragCandidate.pt, mDragCandidate.ent->all_vars());
		}
		mMode = modes::DRAGGING;
	}
	mLastPlPos = plPos;

	return true;
}

void SketchDefault_tool::update_dragCandidate(glm::vec2 cursorPos)
{
	Camera* cam = mEnv->state()->cam;
	Sketch* sk = mEnv->target();
	if(cursorPos != mLastCursorPos && !sk->need_graphicUpdate() && !cam->need_update()) { // Prevent recomputing the same thing multiple times
		mDragCandidate = sk->closest_draggable(cursorPos, cam, DRAWABLE);
		mLastCursorPos = cursorPos;
	}
}
void SketchDefault_tool::update_hover(glm::vec2 cursorPos)
{
	Drawable* hovered_drw = mEnv->target()->closest(cursorPos, mEnv->state()->cam, DRAWABLE).ent;
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