
#include "EnterPart_action.hpp"
#include <Workspaces/Document.hpp>
#include <Bloop.hpp>

EnterPart_action::EnterPart_action(bool set_Camera):
	SwitchWorkspace_action(Bloop::PART, set_Camera),
	mTarget(nullptr)
{

}

bool EnterPart_action::do_work(Document* caller)
{
	SwitchWorkspace_action::do_work(caller);
	mTarget = caller->target();
	if(mTarget && !mTarget->has_volume()) {
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