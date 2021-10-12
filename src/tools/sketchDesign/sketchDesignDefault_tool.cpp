
#include "sketchDesignDefault_tool.hpp"

#include <entities/sketch.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>
#include <actions/common/toggleBaseObject_action.hpp>
#include <actions/common/serial_action.hpp>
#include <actions/sketchDesign/assignPosSnapshots_action.hpp>

#include <iostream>

sketchDesignDefault_tool::sketchDesignDefault_tool(sketchDesign* env):
	tool<sketchDesign>(env),
	// mDraggedEnt(nullptr),
	mHoveredEnt(nullptr),
	mSelectionRect(nullptr),
	mAllowedToMove(false)
{

}

void sketchDesignDefault_tool::init()
{
	if(!mSelectionRect) {
		mSelectionRect = std::make_shared<selectionRectangle>(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), mEnv->target()->basePlane());
	}
	mAllowedToMove = false;
	mMoving = false;
}

bool sketchDesignDefault_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_Escape) {
		unselect_all();
		return false;
	} else if(event->keyval == GDK_KEY_Delete) {
		auto mDeleteAllSelected = std::make_shared<serial_action>();
		sketch_ptr sk = mEnv->target();
		sk->for_each_selected([&](sketchEntity_ptr ent) {
			if(sk->can_delete(ent))
				mDeleteAllSelected->add_action(std::make_shared<toggleBaseObject_action>(ent, false));
		});
		mEnv->state()->doc->push_action(mDeleteAllSelected);
		unselect_all();
	}
	return true;
}

bool sketchDesignDefault_tool::manage_button_press(GdkEventButton* event)
{
	mAllowedToMove = false;
	if(event->button == 1) {
		auto clickedEnt = mEnv->target()->entity_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
		if(clickedEnt) {
			mAllowedToMove = true;
			if(!clickedEnt->selected()) {
				if(!(event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK))
					mEnv->target()->clear_selectedGeometries();
				mEnv->target()->add_selectedGeometry(clickedEnt);
			}
		} else if(!clickedEnt) {
			unselect_all();

			camera_ptr cam = mEnv->state()->cam;
			geom_3d::plane_abstr_ptr pl = mEnv->target()->basePlane();
			glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
			mSelectionRect->set_points(pos, pos);
			mEnv->target()->add_toolPreview(mSelectionRect);
			mStartPos = glm::vec2(event->x, event->y);
		}
	}
	return true;
}
bool sketchDesignDefault_tool::manage_button_release(GdkEventButton* event)
{
	if(event->button == 1)
		mEnv->target()->clear_toolPreview();

	// mDraggedEnt = nullptr;
	
	if(mMoving) {
		int i = 0;
		std::vector<entityPosSnapshot_ptr> final_mov = mEnv->target()->selectedGeometriesSnapshots();
		mEnv->state()->doc->push_action(std::make_shared<assignPosSnapshots_action>(mStartMoveSnapshot, final_mov));
	}

	mAllowedToMove = false;
	mMoving = false;
	return true;
}

bool sketchDesignDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	sketch_ptr sk = mEnv->target();
	DEBUG_ASSERT(sk, "No valid sketch.");

	geom_3d::plane_abstr_ptr pl = sk->basePlane();
	camera_ptr cam = mEnv->state()->cam;
	glm::vec2 mousePos(event->x, event->y);
	glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(mousePos, false)));

	if(event->state & GDK_BUTTON1_MASK) {
		if(mAllowedToMove) {
			if(!mMoving) {
				mMoving = true;
				mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { ent->set_dragged(true); });
				mStartMoveSnapshot = mEnv->target()->selectedGeometriesSnapshots();
			}
			mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { ent->move(mPrevPos, pos, mPrevMousePos - mousePos);	});
			bool update_attempt = sk->update_constraints(true, false);
		} else {
			mSelectionRect->set_endPoint(pos);
			if(event->x > mStartPos.x) {
				mSelectionRect->set_mode(selectionRectangle::COVER);
			} else {
				mSelectionRect->set_mode(selectionRectangle::TOUCH);
			}
			sk->toggle_selection_from_area(mSelectionRect->start(), mSelectionRect->end(), mSelectionRect->mode() == selectionRectangle::COVER);
		}
	} else {
		sketchEntity_ptr candidate = mEnv->target()->entity_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
		if(candidate != mHoveredEnt) {
			if(mHoveredEnt) {
				mHoveredEnt->set_hover(false);
			}
			if(candidate) {
				candidate->set_hover(true);
			}
			mHoveredEnt = candidate;
		}
	}
	mPrevPos = pos;
	mPrevMousePos = mousePos;

	return true;
}


void sketchDesignDefault_tool::unselect_all()
{
	mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { 
		if(ent->dragged())
			ent->set_dragged(false);
	});
	mEnv->target()->clear_selectedGeometries();
}