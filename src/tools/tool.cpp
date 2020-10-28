
#include "tool.hpp"
#include "../bloop.hpp"
#include "../workspaces/workspace.hpp"

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

bool simpleSelector_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState)) {
		(*mWorkspaceState)->selectionBuffer->bind();
		unsigned char data[4] = {0, 0, 0, 0};
		GLCall(glReadPixels(event->x, (*mWorkspaceState)->height - event->y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data));
		std::shared_ptr<entity> ent = (*mWorkspaceState)->indexer->get(glm::ivec3(data[0], data[1], data[2]));

		if(ent != mCurrentHover) {
			if(mCurrentHover) {
				mCurrentHover->set_hover(false);
			}
			if(ent) {
				ent->set_hover(true);
			}
			mCurrentHover = ent;
		}
	}
	return true;
}