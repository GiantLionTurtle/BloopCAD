
#include "QuitPart_action.hpp"

#include <Workspaces/Document.hpp>
#include <Drawables/Containers/Part.hpp>

QuitPart_action::QuitPart_action():
	mTarget(nullptr) // Target is determined when do_work is first called
{

}

bool QuitPart_action::do_work(Document* caller)
{
	mTarget = caller->target();
	if(mTarget) { // Should always be true
		mTarget->hide_origin();
	}
	return true;
}

bool QuitPart_action::undo_work(Document* caller)
{
	if(mTarget && !mTarget->has_volume()) { // Show the origin if no volume is in the part (at time of writing there is no way to add volume)
		mTarget->show_origin();
	}
	return true;
}