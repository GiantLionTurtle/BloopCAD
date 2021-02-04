
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
	simpleSelector_tool(env),
	mDraggedEnt(nullptr),
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
		mDraggedEnt = entity_at_point_derived<sketchEntity>(glm::vec2(event->x, event->y));
	}
	simpleSelector_tool::manage_button_press(event);
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
		if(!sk) {
			LOG_WARNING("No valid state.");
			return true;
		}
		geom_3d::plane_abstr_ptr pl = sk->basePlane();
		camera_ptr cam = mEnv->state()->cam;
		glm::vec2 pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		if(mMoving) {
			sk->backup_system();
			mDraggedEnt->move(glm::vec3(pos, 0.0f), glm::vec3(mPrevPos, 0.0f));
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
	}
	
	simpleSelector_tool::manage_mouse_move(event);
	return true;
}
