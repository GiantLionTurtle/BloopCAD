
#include "enterPartDesign_action.hpp"
#include <document.hpp>
#include <bloop.hpp>

enterPartDesign_action::enterPartDesign_action(bool set_Camera):
	switchWorkspace_action(bloop::PART, set_Camera),
	mTarget(nullptr),
	mInitialTarget(nullptr)
{

}

bool enterPartDesign_action::do_work(document* caller)
{
	switchWorkspace_action::do_work(caller);
	mTarget = caller->target();
	if(mTarget && !mTarget->has_volume()) {
		mTarget->origin()->show();
	}
	mInitialTarget = caller->state()->target;
	caller->state()->target = mTarget;
	return true;
}

bool enterPartDesign_action::undo_work(document* caller)
{
	switchWorkspace_action::undo_work(caller);
	if(mTarget) {
		mTarget->origin()->hide();
	}
	caller->state()->target = mInitialTarget;
	return true;
}