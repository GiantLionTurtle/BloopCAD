
#ifndef ENTERSKETCHDESIGN_ACTION_HPP_
#define ENTERSKETCHDESIGN_ACTION_HPP_

#include <Actions/Common/SwitchWorkspace_action.hpp>
#include <Drawables/Sketch.hpp>
#include <Drawables/tangibleEntities/Plane.hpp>

#include <memory>

class EnterSketch_action : public SwitchWorkspace_action {
private:
	Sketch* mTarget;
	Drawable* mInitialTarget;
public:
	EnterSketch_action(Sketch* target, bool set_Camera = false);

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