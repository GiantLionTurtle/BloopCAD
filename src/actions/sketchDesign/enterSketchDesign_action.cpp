
#include "enterSketchDesign_action.hpp"
#include <document.hpp>

enterSketchDesign_action::enterSketchDesign_action(sketch_ptr target, bool set_camera):
	switchWorkspace_action("sketchDesign", set_camera),
	mPlane(nullptr),
	mTarget(target)
{
	mPlane = std::dynamic_pointer_cast<plane>(target->basePlane());
}

bool enterSketchDesign_action::do_work(document* caller)
{
	switchWorkspace_action::do_work(caller);
	if(mPlane) {
		mPlane->hide();
	}
	mInitialTarget = caller->currentWorkspaceState()->target;
	caller->currentWorkspaceState()->target = mTarget;
	return true;
}

bool enterSketchDesign_action::undo_work(document* caller)
{
	switchWorkspace_action::undo_work(caller);
	if(mPlane) {
		mPlane->show();
	}
	caller->currentWorkspaceState()->target = mInitialTarget;
	return true;
}