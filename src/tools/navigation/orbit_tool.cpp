
#include "orbit_tool.hpp"

#include <workspaces/workspace.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

orbit_tool::orbit_tool(workspace* env):
	tool_abstract(env), 
	is_moving(false),
	tmp_right(0.0f, 0.0f, 0.0f)
{

}

bool orbit_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_q) {
		tmp_right = mEnv->state()->cam->right();
		std::cout<<"Saving right vector: "<<glm::to_string(tmp_right)<<"\n";
	}
	return true;
}

bool orbit_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && (event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK)) {
		glm::vec2 pos(event->x, -event->y);
		if(is_moving) {
			glm::vec2 abs_mov = (pos-prevPos) * 0.005f;
			
			std::shared_ptr<camera> cam = mEnv->state()->cam;
			cam->orientation() += glm::vec3(
				-abs_mov.y, 
				(cam->flipped() ? -abs_mov.x : abs_mov.x), 
				0.0f);
		} else {
			is_moving = true;
		}
		prevPos = pos;
	} else {
		is_moving = false;
	}
	return true;
} 
