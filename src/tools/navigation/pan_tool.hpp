
#ifndef PAN_TOOL_HPP_
#define PAN_TOOL_HPP_

#include <tools/tool.hpp>

/*
	@class pan_tool describes the tool that moves the model 
	@parent : tool_abstract
*/
class pan_tool : public tool_abstract {
private:
	glm::vec2 prevPos; // Helper record of the position of the mouse on the previous update
	bool is_moving; // If the mouse is currently moving
public:
	/*
		@function pan_tool creates an pan_tool object

		@param env : The workspace owning the tool
	*/
	pan_tool(workspace* env);


	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);
	/*
		@function manage_mouse_move moves the model according to the mouse movement if the right condition are met

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "pan"; }
};

#endif