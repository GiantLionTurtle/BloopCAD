
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

bool zoom_tool::manage_button_press(GdkEventButton* event) 
{
	if(event->state & GDK_BUTTON1_MASK) {
		zoomStart = glm::vec2(event->x / (*mWorkspaceState)->width  * 2.0f - 1.0f, 1.0f - event->y / (*mWorkspaceState)->height * 2.0f);
		is_zooming = true; init();
	} 
	return true; 
}

bool zoom_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && is_zooming) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			zoom(zoomStart, -0.02f * (pos.y-prevPos.y));
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
		zoom(glm::vec2(event->x / (*mWorkspaceState)->width  * 2.0f - 1.0f, 1.0f - event->y / (*mWorkspaceState)->height * 2.0f), -0.015f * event->delta_y);
	}
	return true;
}

void zoom_tool::zoom(glm::vec2 origin, float amount)
{
	float scale = 1.0f + amount;
	(*mWorkspaceState)->target->scale(scale);

	glm::vec4 event_pos(origin.x, origin.y, 0.0f, 0.0f);

	glm::mat4 mvp = (*mWorkspaceState)->cam->projection() * (*mWorkspaceState)->cam->view() * (*mWorkspaceState)->target->transform();
	glm::mat4 vp = (*mWorkspaceState)->cam->projection() * (*mWorkspaceState)->cam->view();

	glm::vec4 pos_screen = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float tmp_w = pos_screen.w;
	pos_screen /= pos_screen.w;

	glm::vec4 up_screen = vp * glm::vec4((*mWorkspaceState)->cam->up(), 1.0f);
	up_screen /= up_screen.w;
	glm::vec4 right_screen = vp * glm::vec4((*mWorkspaceState)->cam->right(), 1.0f);
	right_screen /= right_screen.w;

	float up_length = std::sqrt(up_screen.x * up_screen.x + up_screen.y * up_screen.y);
	float right_length = std::sqrt(right_screen.x * right_screen.x + right_screen.y * right_screen.y);

	glm::vec4 delta = event_pos - pos_screen;

	float translate_by_x = (delta.x * -amount) / right_length;
	float translate_by_y = (delta.y * -amount) / up_length;

	(*mWorkspaceState)->target->translate((*mWorkspaceState)->cam->right() * translate_by_x);
	(*mWorkspaceState)->target->translate((*mWorkspaceState)->cam->up() * translate_by_y);
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
			// (*mWorkspaceState)->cam->move_cartesian((*mWorkspaceState)->cam->right() * -abs_mov.x * 0.2f * speed_ratio);
			// (*mWorkspaceState)->cam->move_cartesian((*mWorkspaceState)->cam->up() * -abs_mov.y * 0.2f * speed_ratio);
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