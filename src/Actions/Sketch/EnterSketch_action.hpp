
#ifndef ENTERSKETCHDESIGN_ACTION_HPP_
#define ENTERSKETCHDESIGN_ACTION_HPP_

#include <Actions/Common/SwitchWorkspace_action.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Drawables/3d/Plane.hpp>

#include <memory>

class EnterSketch_action : public SwitchWorkspace_action {
private:
	Sketch* mTarget;
public:
	EnterSketch_action(Sketch* target, bool set_Camera = false);

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