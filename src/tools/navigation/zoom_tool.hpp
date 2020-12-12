
#ifndef ZOOM_TOOL_HPP_
#define ZOOM_TOOL_HPP_

#include <tools/tool.hpp>

/*
	@class zoom_tool describe the tool used to zoom the model
	@parent : tool_abstract
*/
class zoom_tool : public tool_abstract {
private:
	glm::vec2 prevPos; // Helper previous position of mouse, on last update
	glm::vec2 zoomStart; // Position of the mouse when the zoom start (with mouse move zoom)
	bool is_zooming; // Is the model being zoomed
public:
	/*
		@function zoom_tool creates an zoom_tool object

		@param env : The workspace owning the tool
	*/
	zoom_tool(workspace* env);
	
	/*
		@function finish is an override to clean up after the tool stops being used (but not destroyed)
	*/
	virtual void finish();
	/*
		@function init is an override to prepare the tool to be used
	*/
	virtual void init();

	/*
		@function manage_mouse_move zooms the model relative to the y distance of the mouse
		to the zoom starting point if the right conditions are met

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_button_release calls finish when zoom condition stop being met

		@param event : The button event handed by gtk
	*/
	virtual bool manage_button_release(GdkEventButton* event);
	/*
		@function manage_scroll zooms the model around the scroll point

		@param event : The scroll event handed by gtk
	*/
	virtual bool manage_scroll(GdkEventScroll* event);

	/*
		@function zoom scales the model around a point

		@param origin : The scale origin
		@param amount : By how much to scale
	*/
	void zoom(glm::vec2 origin, float amount);
};

#endif