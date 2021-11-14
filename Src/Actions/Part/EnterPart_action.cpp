
#include "EnterPart_action.hpp"
#include <Workspaces/Document.hpp>
#include <Bloop.hpp>

EnterPart_action::EnterPart_action(bool set_camera)
	: SwitchWorkspace_action(Bloop::PART, set_camera)
	, mTarget(nullptr) // Target will be determined when do_work is called since every document has only one part.. this might change
{

}

bool EnterPart_action::do_work(Document* caller)
{
	SwitchWorkspace_action::do_work(caller);
	mTarget = caller->target(); // Get target
	if(mTarget && !mTarget->has_volume()) { // Origin planes, point and axis must be visible if no volume is present (might change to anything)
		mTarget->show_origin();
	}
	return true;
}

bool EnterPart_action::undo_work(Document* caller)
{
	SwitchWorkspace_action::undo_work(caller);
	if(mTarget) {
		mTarget->hide_origin();
	}
	return true;
}