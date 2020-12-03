
#ifndef LINE_TOOL_HPP_
#define LINE_TOOL_HPP_

#include <tools/tool.hpp>

#include <glm/glm.hpp>

class line_tool : public tool_abstract {
private:
	glm::vec3 startPos, endPos;
public:
	line_tool(workspace* env);

	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_button_press(GdkEventButton* event);
};

#endif