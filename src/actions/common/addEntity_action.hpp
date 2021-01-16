
#ifndef ADDENTITY_ACTION_HPP_
#define ADDENTITY_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/entity.hpp>

#include <glm/glm.hpp>

#include <memory>

class addEntity_action : public action {
private:
	entity_ptr mChildEnt, mParentEnt;
	bool mValid;
public:
	addEntity_action(entity_ptr child, entity_ptr parent);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);

	entity_ptr child_entity() const { return mChildEnt; }
	entity_ptr parent_entity() const { return mParentEnt; }
};

#endif