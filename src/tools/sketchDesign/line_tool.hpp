
#ifndef LINE_TOOL_HPP_
#define LINE_TOOL_HPP_

#include <tools/tool.hpp>

#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>

#include <glm/glm.hpp>

/*
	@class line_tool describes the tool used to make lines and polylines on a sketch
	@parent : tool_abstract
*/
class line_tool : public tool_abstract {
private:
	sketchPoint_ptr mStartPos, mEndPos; // start and end pos of the line
	sketchLine_ptr mLine;
	bool started; // Whether or not the drawing of the line has started
public:
	/*
		@function line_tool creates a line_tool object

		@param env : The workspace that owns the tool
	*/
	line_tool(workspace* env);

	/*
		@function init makes sure the started flag is down
	*/
	virtual void init();

	/*
		@function manage_mouse_move will update a temporary line to display a non placed line

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_button_press starts or end&push lines on sketch

		@param event : The button press event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "line"; }
};

#endif