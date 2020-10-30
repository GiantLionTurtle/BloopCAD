
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
			float scale = 1.0f - 0.02f * (pos.y-prevPos.y);
			(*mWorkspaceState)->target->scale(glm::vec3(scale, scale, scale));
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
		float scale = 1.0f - 0.015f * event->delta_y;
		(*mWorkspaceState)->target->scale(glm::vec3(scale, scale, scale));
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

bool pan_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && event->state & GDK_BUTTON1_MASK) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov(pos.x-prevPos.x, pos.y-prevPos.y);
			// (*mWorkspaceState)->cam->change_zoom((pos.y-prevPos.y) * 2.0f);
			// std::cout<<"Right: "<<glm::to_string((*mWorkspaceState)->cam->right())<<"\n";
			// std::cout<<"Up: "<<glm::to_string((*mWorkspaceState)->cam->up())<<"\n";
			float speed_ratio = 1.0f / (*mWorkspaceState)->cam->zoom();
			(*mWorkspaceState)->cam->move_cartesian((*mWorkspaceState)->cam->right() * -abs_mov.x * 0.2f * speed_ratio);
			(*mWorkspaceState)->cam->move_cartesian((*mWorkspaceState)->cam->up() * -abs_mov.y * 0.2f * speed_ratio);
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
}