
#include "zoom_tool.hpp"

#include "../../workspaces/workspace.hpp"

bool zoom_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState) && event->state & GDK_BUTTON1_MASK) {
		if(!is_zooming) {
			is_zooming = true;
			zoomStart = glm::vec2(event->x / (*mWorkspaceState)->width  * 2.0f - 1.0f, 1.0f - event->y / (*mWorkspaceState)->height * 2.0f);
		}
		glm::vec2 pos(event->x, -event->y);

		float delta = pos.y - prevPos.y;
		if(is_moving) {
			zoom(zoomStart, 0.02f * delta);
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