
#include "enableEntity_action.hpp"
#include <utils/errorLogger.hpp>
#include <document.hpp>

enableEntity_action::enableEntity_action(entity_ptr ent):
	mEnt(ent)
{
	DEBUG_ASSERT(ent, "Invalid entity.");
	mEnt->set_exists(false); // It should not exist until the action is consummed
}

bool enableEntity_action::do_work(document* caller)
{
	mEnt->set_exists(true); // The line now exists
	return true;
}
bool enableEntity_action::undo_work(document* caller)
{
	mEnt->set_exists(false); // The line now doesn't exist, but is still available if needed
	return true;
}