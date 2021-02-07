
#ifndef ENABLEENTITY_ACTION_HPP_
#define ENABLEENTITY_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/entity.hpp>

#include <glm/glm.hpp>

#include <memory>

class enableEntity_action : public action {
private:
	entity_ptr mEnt;
public:
	enableEntity_action(entity_ptr ent);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif