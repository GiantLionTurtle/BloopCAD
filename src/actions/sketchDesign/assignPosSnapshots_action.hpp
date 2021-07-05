
#ifndef ASSIGNPOSSNAPSHOT_ACTION_HPP_
#define ASSIGNPOSSNAPSHOT_ACTION_HPP_

#include <actions/action.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchGeometry.hpp>

class assignPosSnapshots_action : public action {
private:
	std::vector<entityPosSnapshot_ptr> mSourceSnapshots, mTargetSnapshots;
public:
	assignPosSnapshots_action(entityPosSnapshot_ptr source, entityPosSnapshot_ptr target);
	assignPosSnapshots_action(std::vector<entityPosSnapshot_ptr> source, std::vector<entityPosSnapshot_ptr> target);
	assignPosSnapshots_action(entityPosSnapshot_ptr target);
	assignPosSnapshots_action(std::vector<entityPosSnapshot_ptr> target);


	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif