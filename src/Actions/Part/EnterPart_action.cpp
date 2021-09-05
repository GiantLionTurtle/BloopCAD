
#include "EnterPart_action.hpp"
#include <document.hpp>
#include <bloop.hpp>

EnterPart_action::EnterPart_action(bool set_Camera):
	SwitchWorkspace_action(bloop::PART, set_Camera),
	mTarget(nullptr)
{

}

bool EnterPart_action::do_work(document* caller)
{
	SwitchWorkspace_action::do_work(caller);
	mTarget = caller->target();
	if(mTarget && !mTarget->has_volume()) {
		mTarget->show_origin();
	}
	return true;
}

bool EnterPart_action::undo_work(document* caller)
{
	SwitchWorkspace_action::undo_work(caller);
	if(mTarget) {
		mTarget->hide_origin();
	}
	return true;
}