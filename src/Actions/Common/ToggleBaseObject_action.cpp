
#include "ToggleBaseObject_action.hpp"
#include <Utils/Debug_util.hpp>
#include <document.hpp>

ToggleBaseObject_action::ToggleBaseObject_action(baseObject* obj, bool toggle_to):
	mObj(obj),
	mToggle_to(toggle_to)
{
	DEBUG_ASSERT(obj, "Invalid object.");
}

bool ToggleBaseObject_action::do_work(document* caller)
{
	mObj->set_exists(mToggle_to);
	return true;
}
bool ToggleBaseObject_action::undo_work(document* caller)
{
	mObj->set_exists(!mToggle_to);
	return true;
}