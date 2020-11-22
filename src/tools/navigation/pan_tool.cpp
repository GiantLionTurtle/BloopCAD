
#include "pan_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>

pan_tool::pan_tool(workspace* env): 
	tool_abstract(env) 
{

}

bool pan_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov(pos.x-prevPos.x, pos.y-prevPos.y);
			float speed_ratio = (float)mEnv->state()->cam->zoom() * 0.02f;
			mEnv->state()->cam->transformation().translation += speed_ratio * glm::vec3(abs_mov.x, abs_mov.y, 0.0f);
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
}