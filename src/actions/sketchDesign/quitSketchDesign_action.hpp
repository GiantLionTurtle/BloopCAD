
#ifndef QUITSKETCHDESIGN_ACTION_HPP_
#define QUITSKETCHDESIGN_ACTION_HPP_

#include <actions/action.hpp>
#include <Drawables/Sketch.hpp>
#include <Drawables/tangibleEntities/Plane.hpp>

#include <memory>

class quitSketchDesign_action : public action {
public:
	quitSketchDesign_action(Sketch* target);

	/*
		@function do_work switches to desired workspace if it exists
	*/
	virtual bool do_work(document* caller);
	/*
		@function undo_work revert back to the initial workspace
	*/
	virtual bool undo_work(document* caller);
};

#endif