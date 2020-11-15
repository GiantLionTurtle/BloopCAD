
#ifndef PAN_TOOL_HPP_
#define PAN_TOOL_HPP_

#include <tools/tool.hpp>

class pan_tool : public tool_abstract {
private:
	glm::vec2 prevPos;
	bool is_moving;
public:
	pan_tool(workspace* env);

	virtual bool manage_mouse_move(GdkEventMotion* event);
};

#endif