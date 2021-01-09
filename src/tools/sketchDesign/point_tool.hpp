
#ifndef POINT_TOOL_HPP_
#define POINT_TOOL_HPP_

#include <tools/tool.hpp>

#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>

#include <glm/glm.hpp>

class point_tool : public tool_abstract {
private:
	sketchPoint_ptr mCurrentPoint;
public:
	/*
		@function line_tool creates a line_tool object

		@param env : The workspace that owns the tool
	*/
	point_tool(workspace* env);

	/*
		@function manage_button_press starts or end&push lines on sketch

		@param event : The button press event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event);
};

#endif