
#ifndef QUITPARTDESIGN_ACTION_HPP_
#define QUITPARTDESIGN_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/part.hpp>

#include <memory>

class quitPartDesign_action : public action {
private:
	std::shared_ptr<part> mTarget;
public:
	quitPartDesign_action();

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