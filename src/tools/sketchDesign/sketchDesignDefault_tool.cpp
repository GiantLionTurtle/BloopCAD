
#include "sketchDesignDefault_tool.hpp"

#include <entities/sketch.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>

#include <iostream>

sketchDesignDefault_tool::sketchDesignDefault_tool(workspace* env):
	simpleSelector_tool(env)
{

}

bool sketchDesignDefault_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(event->state & GDK_BUTTON1_MASK) {
		sketchPoint_ptr hover_pt = std::dynamic_pointer_cast<sketchPoint>(mCurrentHover);
		if(hover_pt) {
			sketch_ptr sk = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);
			if(!sk) {
				LOG_WARNING("No valid state.");
				return true;
			}
			plane_abstract_ptr pl = sk->basePlane();
			camera_ptr cam = mEnv->state()->cam;
			glm::vec2 point_pos = pl->point_3d_to_2d(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
			sk->backup_system();
			hover_pt->set_pos(point_pos);
			hover_pt->set_tmpConstant(true); 	
			if(!sk->update_constraints()) {
				hover_pt->set_tmpConstant(false);
				if(!sk->update_constraints())
					sk->revert_system_to_backup();
			}
			hover_pt->set_tmpConstant(false);
		}
	}
	simpleSelector_tool::manage_mouse_move(event);
	return true;
}
