
#ifndef ENTERPARTDESIGN_ACTION_HPP_
#define ENTERPARTHDESIGN_ACTION_HPP_

#include <Actions/Common/SwitchWorkspace_action.hpp>
#include <Drawables/Part.hpp>

#include <memory>

class EnterPart_action : public SwitchWorkspace_action {
private:
	Part* mTarget;
	Drawable* mInitialTarget;
public:
	EnterPart_action(bool set_Camera = false);

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