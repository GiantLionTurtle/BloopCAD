
#ifndef DISABLEENTITY_ACTION_HPP_
#define ENABLEENTITY_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/entity.hpp>

class disableEntity_action : public action {
private:
	entity_ptr mEnt;
public:
	disableEntity_action(entity_ptr ent);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif