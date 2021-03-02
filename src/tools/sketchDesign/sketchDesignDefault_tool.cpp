
#include "sketchDesignDefault_tool.hpp"

#include <entities/sketch.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>

#include <iostream>

sketchDesignDefault_tool::sketchDesignDefault_tool(sketchDesign* env):
	tool<sketchDesign>(env),
	mDraggedEnt(nullptr),
	mHoveredEnt(nullptr),
	mMoving(false)
{

}

void sketchDesignDefault_tool::init()
{
	mDraggedEnt = nullptr;
	mMoving = false;
}

bool sketchDesignDefault_tool::manage_button_press(GdkEventButton* event)
{
	if(event->button == 1) {
		mDraggedEnt = mEnv->target()->geometry_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
		if(mDraggedEnt && !mDraggedEnt->selected()) {
			if(!(event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK))
				mEnv->target()->clear_selectedGeometries();
			mEnv->target()->add_selectedGeometry(mDraggedEnt);
		} else if(!mDraggedEnt) {
			mEnv->target()->clear_selectedGeometries();
		}
	}
	return true;
}
bool sketchDesignDefault_tool::manage_button_release(GdkEventButton* event)
{
	mDraggedEnt = nullptr;
	mMoving = false;
	return true;
}
bool sketchDesignDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mDraggedEnt) {
		sketch_ptr sk = mEnv->target();
		DEBUG_ASSERT(sk, "No valid sketch.");

		geom_3d::plane_abstr_ptr pl = sk->basePlane();
		camera_ptr cam = mEnv->state()->cam;
		glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		if(mMoving) {
			mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { 
				ent->move(mPrevPos, pos); 
				ent->set_tmpConstant(true);
			});
			bool update_attempt = sk->update_constraints();
			mEnv->target()->for_each_selected([&](sketchEntity_ptr ent) { 
				ent->set_tmpConstant(false);
			});
			if(!update_attempt) {
				LOG_WARNING("Failed update, attempting to recover.");
				update_attempt = sk->update_constraints();
				if(!update_attempt)
					LOG_WARNING("Could not recover from previous error.");
			}
		}
		mMoving = true;
		mPrevPos = pos;
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
	return true;
}
