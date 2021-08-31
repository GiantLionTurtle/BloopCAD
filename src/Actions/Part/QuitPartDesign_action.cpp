
#include "QuitPartDesign_action.hpp"
#include <document.hpp>

QuitPartDesign_action::QuitPartDesign_action():
	mTarget(nullptr)
{

}

bool QuitPartDesign_action::do_work(document* caller)
{
	mTarget = caller->target();
	if(mTarget) {
		mTarget->hide_origin();
	}
	return true;
}

bool QuitPartDesign_action::undo_work(document* caller)
{
	if(mTarget && !mTarget->has_volume()) {
		mTarget->show_origin();
	}
	return true;
}