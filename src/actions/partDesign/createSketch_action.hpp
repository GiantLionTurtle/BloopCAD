
#ifndef CREATESKETCH_ACTION_HPP_
#define CREATESKETCH_ACRION_HPP_

#include <actions/action.hpp>
#include <entities/plane_abstract.hpp>
#include <entities/part.hpp>
#include <entities/sketch.hpp>

#include <memory>

/*
	@class createSketch_action is an action to add a sketch to a part, given a part and a base plane
	@parent : action
*/
class createSketch_action : public action {
private:
	std::shared_ptr<plane_abstract> mSketchPlane; 	// The baseplane of the sketch
	std::shared_ptr<part> mTarget; 					// The part onto which to add the plane
	std::shared_ptr<sketch> mSketch;				// The created sketch
public:
	/*
		@function createSketch_action creates the action 
		
		@param sketchPlane: The plane that describes the sketch
		@param target:		The part onto which to add the sketch
	*/
	createSketch_action(std::shared_ptr<plane_abstract> sketchPlane, std::shared_ptr<part> target);

	/*
		@function do_work activates the created sketch
	*/
	virtual void do_work();
	/*
		@function undo_work desactivate the created sketch
	*/
	virtual void undo_work();
};

#endif