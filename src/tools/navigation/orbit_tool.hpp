
#ifndef ORBIT_TOOL_HPP_
#define ORBIT_TOOL_HPP_

#include <tools/tool.hpp>

class orbit_tool : public tool_abstract {
private:
	glm::vec2 prevPos;
	bool is_moving;
public:
	orbit_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool_abstract(workspaceState_), is_moving(false) {};

	virtual void finish() { is_moving = false; }
	virtual void init() { is_moving = false; }
	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_button_release(GdkEventButton* event) { if(event->state & GDK_BUTTON1_MASK) finish(); return true; }
};

#endif