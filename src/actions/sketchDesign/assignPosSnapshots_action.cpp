

#include "assignPosSnapshots_action.hpp"
#include <Drawables/tangibleEntities/sketchEntities/entityPosSnapshot_abstract.hpp>

assignPosSnapshots_action::assignPosSnapshots_action(entityPosSnapshot_ptr source, entityPosSnapshot_ptr target):
	mSourceSnapshots({source}),
	mTargetSnapshots({target})
{

}
assignPosSnapshots_action::assignPosSnapshots_action(std::vector<entityPosSnapshot_ptr> source, std::vector<entityPosSnapshot_ptr> target):
	mSourceSnapshots(source),
	mTargetSnapshots(target)
{

}
assignPosSnapshots_action::assignPosSnapshots_action(entityPosSnapshot_ptr target):
	mSourceSnapshots({target}),
	mTargetSnapshots({target})
{

}
assignPosSnapshots_action::assignPosSnapshots_action(std::vector<entityPosSnapshot_ptr> target):
	mSourceSnapshots(target),
	mTargetSnapshots(target)
{

}

bool assignPosSnapshots_action::do_work(document* caller)
{
	for(auto snp : mTargetSnapshots) {
		snp->apply();
	}
	return true;
}
bool assignPosSnapshots_action::undo_work(document* caller)
{
	for(auto snp : mSourceSnapshots) {
		snp->apply();
	}
	return true;
}