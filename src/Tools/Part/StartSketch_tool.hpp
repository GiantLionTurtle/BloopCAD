
#ifndef STARTSKETCH_TOOL_HPP_
#define STARTSKETCH_TOOL_HPP_

#include <Tools/Common/SimpleSelector_tool.hpp>
#include <workspaces/partDesign.hpp>

struct CameraState;
class Sketch;

/*
	@class StartSketch_tool describes the tool used to create a sketch in a part by selecting a plane
	@parent : SimpleSelector_tool

	@note : The plane it is hovering are notified via the SimpleSelector_tool inheritance
*/
class StartSketch_tool : public SimpleSelector_tool<partDesign> {
private:
	Sketch* mCurrentSketch;
public:
	/*
		@function StartSketch_tool creates a StartSketch_tool object

		@param env : The workspace that owns the tool
	*/
	StartSketch_tool(partDesign* env);

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

	virtual void act_on_entity(Drawable* ent);

	virtual std::string name() { return "start sketch"; }
// private:
	/*
		@function start_sketch is the core of the tool, it creates a sketch with a plane and a Camera state
		it will try to make the pick and invert the w and v vectors of the base plane for the sketch plane 
		to best fit the Camera and spare unecesary movements

		@param sketchPlane :	The plane on which the plane will sit
		@param camState_ : 		The state of the Camera at selection time
	*/
	void start_sketch(Geom3d::plane_abstr* sketchPlane, CameraState const& camState_);
};

#endif