
#include "addEntity_action.hpp"
#include <entities/sketch.hpp>
#include <entities/tangibleEntities/line.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>
#include <document.hpp>

addEntity_action::addEntity_action(entity_ptr child, entity_ptr parent):
	mChildEnt(child),
	mParentEnt(parent),
	mValid(child && parent)
{
	if(mValid) {
		mParentEnt->add(mChildEnt);
		mChildEnt->set_exists(false); // It should not exist until the action is consummed
	}
}

bool addEntity_action::do_work()
{
	if(mValid) 
		mChildEnt->set_exists(true); // The line now exists
	return true;
}
bool addEntity_action::undo_work()
{
	if(mValid)
		mChildEnt->set_exists(false); // The line now doesn't exist, but is still available if needed
	return true;
}