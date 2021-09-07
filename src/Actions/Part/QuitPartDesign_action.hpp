
#ifndef QUITPARTDESIGN_ACTION_HPP_
#define QUITPARTDESIGN_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Drawables/Containers/Part.hpp>

#include <memory>

class QuitPartDesign_action : public Action {
private:
	Part* mTarget;
public:
	QuitPartDesign_action();

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