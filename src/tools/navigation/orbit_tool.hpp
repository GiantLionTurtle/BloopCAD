
#ifndef ORBIT_TOOL_HPP_
#define ORBIT_TOOL_HPP_

#include <tools/tool.hpp>

class orbit_tool : public tool_abstract {
private:
	glm::vec2 prevPos;
	bool is_moving;
	glm::vec3 tmp_right;
public:
	orbit_tool(workspace* env);

	virtual void finish() { is_moving = false; }
	virtual void init();
	virtual bool manage_key_press(GdkEventKey* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_button_release(GdkEventButton* event) { if(event->state & GDK_BUTTON1_MASK) finish(); return true; }
};

#endif