
#include "quitPartDesign_action.hpp"
#include <document.hpp>

quitPartDesign_action::quitPartDesign_action():
	mTarget(nullptr)
{

}

bool quitPartDesign_action::do_work(document* caller)
{
	mTarget = caller->target();
	if(mTarget) {
		mTarget->hide_origin();
	}
	return true;
}

bool quitPartDesign_action::undo_work(document* caller)
{
	if(mTarget && !mTarget->has_volume()) {
		mTarget->show_origin();
	}
	return true;
}