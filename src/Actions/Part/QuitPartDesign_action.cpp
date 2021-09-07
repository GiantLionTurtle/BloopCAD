
#include "QuitPartDesign_action.hpp"
#include <Workspaces/Document.hpp>

QuitPartDesign_action::QuitPartDesign_action():
	mTarget(nullptr)
{

}

bool QuitPartDesign_action::do_work(Document* caller)
{
	mTarget = caller->target();
	if(mTarget) {
		mTarget->hide_origin();
	}
	return true;
}

bool QuitPartDesign_action::undo_work(Document* caller)
{
	if(mTarget && !mTarget->has_volume()) {
		mTarget->show_origin();
	}
	return true;
}