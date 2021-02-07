
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
	if(event->button == 1)
		mDraggedEnt = mEnv->target()->geometry_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
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
			sk->backup_system();
			mDraggedEnt->move(mPrevPos, pos);
			mDraggedEnt->set_tmpConstant(true); 	
			if(!sk->update_constraints()) {
				mDraggedEnt->set_tmpConstant(false);
				if(!sk->update_constraints())
					sk->revert_system_to_backup();
			}
			mDraggedEnt->set_tmpConstant(false);
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
