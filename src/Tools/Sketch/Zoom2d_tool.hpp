
#ifndef ZOOM2D_TOOL_HPP_
#define ZOOM2D_TOOL_HPP_

#include <Tools/Tool.hpp>
#include <Workspaces/Sketch_ws.hpp>

class Zoom2d_tool : public tool<Sketch_ws> {
private:
	glm::vec2 mPrevPos; // Helper previous position of mouse, on last update
	glm::vec3 mZoomStart_world;
	bool mIsZooming; // Is the model being zoomed
public:
	/*
		@function Zoom3d_tool creates an Zoom3d_tool object

		@param env : The workspace owning the tool
	*/
	Zoom2d_tool(Sketch_ws* env);
	
	/*
		@function finish is an override to clean up after the tool stops being used (but not destroyed)
	*/
	virtual void finish();
	/*
		@function init is an override to prepare the tool to be used
	*/
	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);

	/*
		@function manage_mouse_move zooms the model relative to the y distance of the mouse
		to the zoom starting point if the right conditions are met

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_scroll zooms the model around the scroll point

		@param event : The scroll event handed by gtk
	*/
	virtual bool manage_scroll(GdkEventScroll* event);

	virtual std::string name() { return "zoom2d"; }

	void zoom(glm::vec3 world_origin, float amount);
};

#endif