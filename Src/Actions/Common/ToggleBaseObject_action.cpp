
#include "ToggleBaseObject_action.hpp"
#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>

ToggleBaseObject_action::ToggleBaseObject_action(BaseObject* obj, bool toggle_to):
	mObj(obj),
	mToggle_to(toggle_to)
{
	DEBUG_ASSERT(obj, "Invalid object.");
}

bool ToggleBaseObject_action::do_work(Document* caller)
{
	mObj->set_exists(mToggle_to); // Notify the object of it's faith
	return true;
}
bool ToggleBaseObject_action::undo_work(Document* caller)
{
	mObj->set_exists(!mToggle_to); // Notify the object of it's faith
	return true;
}