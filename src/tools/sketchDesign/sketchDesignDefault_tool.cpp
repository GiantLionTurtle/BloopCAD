
#include "sketchDesignDefault_tool.hpp"

#include <entities/sketch.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>
#include <actions/common/disableEntity_action.hpp>
#include <actions/common/serial_action.hpp>

#include <iostream>

sketchDesignDefault_tool::sketchDesignDefault_tool(sketchDesign* env):
	tool<sketchDesign>(env),
	// mDraggedEnt(nullptr),
	mHoveredEnt(nullptr),
	mSelectionRect(nullptr),
	mMoving(false)
{

}

void sketchDesignDefault_tool::init()
{
	if(!mSelectionRect) {
		mSelectionRect = std::make_shared<selectionRectangle>(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), mEnv->target()->basePlane());
	}
	// mDraggedEnt = nullptr;
	mMoving = false;
}

bool sketchDesignDefault_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_Escape) {
		unselect_all();
		return false;
	}
	// } else if(event->keyval == GDK_KEY_Delete) {
	// 	auto mDeleteAllSelected = std::make_shared<serial_action>();
	// 	mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) {
	// 		mDeleteAllSelected->add_action(std::make_shared<disableEntity_action>(ent));
	// 	});
	// 	mEnv->state()->doc->push_action(mDeleteAllSelected);
	// }
	return true;
}

bool sketchDesignDefault_tool::manage_button_press(GdkEventButton* event)
{
	mMoving = false;
	if(event->button == 1) {
		auto clickedEnt = mEnv->target()->geometry_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
		if(clickedEnt) {
			mMoving = true;
			if(!clickedEnt->selected()) {
				if(!(event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK))
					mEnv->target()->clear_selectedGeometries();
				mEnv->target()->add_selectedGeometry(clickedEnt);
			}
			mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { ent->set_dragged(true); });
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
	mMoving = false;
	return true;
}

bool sketchDesignDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	sketch_ptr sk = mEnv->target();
	DEBUG_ASSERT(sk, "No valid sketch.");

	geom_3d::plane_abstr_ptr pl = sk->basePlane();
	camera_ptr cam = mEnv->state()->cam;
	glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(event->state & GDK_BUTTON1_MASK) {
		if(mMoving) {

			mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { ent->move(mPrevPos, pos);	});
			bool update_attempt = sk->update_constraints();
			// mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { ent->set_dragged(false); });
			// if(mEnv->target()->n_selectedGeometries() == 1) {
				
			// }
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
		sketchEntity_ptr candidate = mEnv->target()->geometry_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
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