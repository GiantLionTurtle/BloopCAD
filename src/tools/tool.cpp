
#include "tool.hpp"
#include "../bloop.hpp"
#include "../workspaces/workspace.hpp"

bool orbit_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && (event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK)) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			(*mWorkspaceState)->cam->move_spherical(glm::vec3(0, (pos-prevPos) * 0.005f));
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
} 

bool zoom_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && event->state & GDK_BUTTON1_MASK) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			(*mWorkspaceState)->cam->change_zoom((pos.y-prevPos.y) * 2.0f);
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
} 

bool zoom_tool::manage_scroll(GdkEventScroll* event)
{
	if((*mWorkspaceState)) {
		(*mWorkspaceState)->cam->change_zoom(event->delta_y * 1.5);
	}
	return true;
}