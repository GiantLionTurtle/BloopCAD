
#include "pan_tool.hpp"

#include "../../workspaces/workspace.hpp"

bool pan_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && event->state & GDK_BUTTON1_MASK) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov(pos.x-prevPos.x, pos.y-prevPos.y);
			float speed_ratio = 1.0f / (*mWorkspaceState)->cam->zoom();
			(*mWorkspaceState)->target->translate((*mWorkspaceState)->cam->right() 	* abs_mov.x * 0.2f * speed_ratio);
			(*mWorkspaceState)->target->translate((*mWorkspaceState)->cam->up() 	* abs_mov.y * 0.2f * speed_ratio);
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
}