
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
		mTarget->indexer().origin().hide();
	}
	return true;
}

bool quitPartDesign_action::undo_work(document* caller)
{
	if(mTarget && !mTarget->has_volume()) {
		mTarget->indexer().origin().show();
	}
	return true;
}