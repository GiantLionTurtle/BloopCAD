
#ifndef ORBIT_TOOL_HPP_
#define ORBIT_TOOL_HPP_

#include <tools/tool.hpp>

/*
	@class orbit_tool describes the orbit function to rotate the camera around the center of the scene
	@parent : tool_abstract
*/
class orbit_tool : public tool_abstract {
private:
	glm::vec2 prevPos; // Helper record of the position of the mouse on the previous update
	bool is_moving; // If the mouse is currently moving
public:
	/*
		@function orbit_tool creates an orbit_tool object

		@param env : The workspace owning the tool
	*/
	orbit_tool(workspace* env);

	/*
		@function finish is an override to clean up after the tool stops being used (but not destroyed)
	*/
	virtual void finish();
	/*
		@function init is an override to prepare the tool to be used
	*/
	virtual void init();

	/*
		@function manage_mouse_move moves the camera around if the right conditions are met

		@param event : The event handed out by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_button_release calls finish if the middle mouse is released

		@param event : The event handed out by gtk
	*/
	virtual bool manage_button_release(GdkEventButton* event);
};

#endif