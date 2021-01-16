
#include "enterPartDesign_action.hpp"
#include <document.hpp>

enterPartDesign_action::enterPartDesign_action(bool set_camera):
	switchWorkspace_action("partDesign", set_camera),
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
	mInitialTarget = caller->currentWorkspaceState()->target;
	caller->currentWorkspaceState()->target = mTarget;
	return true;
}

bool enterPartDesign_action::undo_work(document* caller)
{
	switchWorkspace_action::undo_work(caller);
	if(mTarget) {
		mTarget->origin()->hide();
	}
	caller->currentWorkspaceState()->target = mInitialTarget;
	return true;
}