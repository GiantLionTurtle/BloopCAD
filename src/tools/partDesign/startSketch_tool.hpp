
#ifndef STARTSKETCH_TOOL_HPP_
#define STARTSKETCH_TOOL_HPP_

#include <tools/common_tools/planeSelector_tool.hpp>

struct camState;

/*
	@class startSketch_tool describes the tool used to create a sketch in a part by selecting a plane
	@parent : planeSelector_tool

	@note : The plane it is hovering are notified via the planeSelector_tool inheritance
*/
class startSketch_tool : public planeSelector_tool {
public:
	/*
		@function startSketch_tool creates a startSketch_tool object

		@param env : The workspace that owns the tool
	*/
	startSketch_tool(workspace* env);

	/*
		@function init checks if a plane has already been selected when the tool was activated
		if yes, it creates a sketch on said plane
	*/
	virtual void init();

	/*
		@function manage_button_press creates a sketch if a plane is under the mouse

		@param event : The button press handed by gtk
	*/
   	virtual bool manage_button_press(GdkEventButton* event);

	virtual void act_on_entity(std::shared_ptr<entity> ent);
private:
	/*
		@function start_sketch is the core of the tool, it creates a sketch with a plane and a camera state
		it will try to make the pick and invert the w and v vectors of the base plane for the sketch plane 
		to best fit the camera and spare unecesary movements

		@param sketchPlane :	The plane on which the plane will sit
		@param camState_ : 		The state of the camera at selection time
	*/
	void start_sketch(std::shared_ptr<plane_abstract> sketchPlane, camState const& camState_);
};

#endif