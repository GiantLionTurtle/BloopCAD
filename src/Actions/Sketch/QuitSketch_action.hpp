
#ifndef QUITSKETCHDESIGN_ACTION_HPP_
#define QUITSKETCHDESIGN_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Drawables/3d/Plane.hpp>

#include <memory>

class QuitSketch_action : public Action {
public:
	QuitSketch_action(Sketch* target);

	/*
		@function do_work switches to desired workspace if it exists
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work revert back to the initial workspace
	*/
	virtual bool undo_work(Document* caller);
};

#endif