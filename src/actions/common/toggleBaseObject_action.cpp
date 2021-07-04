
#include "toggleBaseObject_action.hpp"
#include <utils/DebugUtils.hpp>
#include <document.hpp>

toggleBaseObject_action::toggleBaseObject_action(baseObject_ptr obj, bool toggle_to):
	mObj(obj),
	mToggle_to(toggle_to)
{
	DEBUG_ASSERT(obj, "Invalid object.");
}

bool toggleBaseObject_action::do_work(document* caller)
{
	mObj->set_exists(mToggle_to);
	return true;
}
bool toggleBaseObject_action::undo_work(document* caller)
{
	mObj->set_exists(!mToggle_to);
	return true;
}