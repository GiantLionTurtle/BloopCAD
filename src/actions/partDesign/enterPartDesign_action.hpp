
#ifndef ENTERPARTDESIGN_ACTION_HPP_
#define ENTERPARTHDESIGN_ACTION_HPP_

#include <actions/common/switchWorkspace_action.hpp>
#include <entities/part.hpp>

#include <memory>

class enterPartDesign_action : public switchWorkspace_action {
private:
	std::shared_ptr<part> mTarget;
	entity_ptr mInitialTarget;
public:
	enterPartDesign_action(bool set_Camera = false);

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