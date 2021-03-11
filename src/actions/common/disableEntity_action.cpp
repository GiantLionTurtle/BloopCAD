
#include "disableEntity_action.hpp"
#include <utils/errorLogger.hpp>
#include <document.hpp>

disableEntity_action::disableEntity_action(entity_ptr ent):
	mEnt(ent)
{
	DEBUG_ASSERT(ent, "Invalid entity.");
	// mEnt->set_exists(true); // It should not exist until the action is consummed
}

bool disableEntity_action::do_work(document* caller)
{
	mEnt->set_exists(false);
	return true;
}
bool disableEntity_action::undo_work(document* caller)
{
	mEnt->set_exists(true);
	return true;
}