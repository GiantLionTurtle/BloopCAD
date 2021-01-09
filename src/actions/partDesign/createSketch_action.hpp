
#ifndef CREATESKETCH_ACTION_HPP_
#define CREATESKETCH_ACRION_HPP_

#include <actions/action.hpp>
#include <entities/geometry/plane_abstract.hpp>
#include <entities/part.hpp>
#include <entities/sketch.hpp>

#include <memory>

/*
	@class createSketch_action is an action to add a sketch to a part, given a part and a base plane
	@parent : action
*/
class createSketch_action : public action {
private:
	plane_abstract_ptr mSketchPlane; 	// The baseplane of the sketch
	std::shared_ptr<part> mTarget; 					// The part onto which to add the plane
	sketch_ptr mSketch;				// The created sketch
public:
	/*
		@function createSketch_action creates the action 
		
		@param sketchPlane: The plane that describes the sketch
		@param target:		The part onto which to add the sketch
	*/
	createSketch_action(plane_abstract_ptr sketchPlane, std::shared_ptr<part> target);

	/*
		@function do_work activates the created sketch
	*/
	virtual bool do_work();
	/*
		@function undo_work desactivate the created sketch
	*/
	virtual bool undo_work();
};

#endif