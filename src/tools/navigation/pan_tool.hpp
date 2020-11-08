
#ifndef PAN_TOOL_HPP_
#define PAN_TOOL_HPP_

#include "../tool.hpp"

class pan_tool : public tool_abstract {
private:
	glm::vec2 prevPos;
	bool is_moving;
public:
	pan_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool_abstract(workspaceState_) {};

	virtual bool manage_mouse_move(GdkEventMotion* event);
};

#endif