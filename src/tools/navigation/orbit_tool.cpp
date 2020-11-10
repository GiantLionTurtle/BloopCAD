
#include "orbit_tool.hpp"

#include <workspaces/workspace.hpp>

bool orbit_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && (event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK)) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov = (pos-prevPos) * 0.005f;
			(*mWorkspaceState)->cam->move_spherical(glm::vec3(0, (*mWorkspaceState)->cam->flipped() ? -abs_mov.x : abs_mov.x, abs_mov.y));
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
} 
