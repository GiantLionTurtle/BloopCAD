
#ifndef PAN2D_TOOL_HPP_
#define PAN2D_TOOL_HPP_

#include <workspaces/sketchDesign.hpp>
#include <tools/tool.hpp>

class pan2d_tool : public tool<sketchDesign> {
private:
	glm::vec3 mTranStart, mDragStart; // Helper record of the position of the mouse on the previous update
	glm::vec3 mPl_right, mPl_up;
	bool is_moving; // If the mouse is currently moving
	Camera_ptr mProxyCam; // Virtual Camera used to calculate the pan offset
public:
	/*
		@function pan_tool creates an pan_tool object

		@param env : The workspace owning the tool
	*/
	pan2d_tool(sketchDesign* env);

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);
	/*
		@function manage_mouse_move moves the model according to the mouse movement if the right condition are met

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "pan2d"; }
};

#endif