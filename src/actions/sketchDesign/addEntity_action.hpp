
#ifndef ADDLINE_ACTION_HPP_
#define ADDLINE_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/entity.hpp>

#include <glm/glm.hpp>

#include <memory>

class document; // Declaration of document needed to declare pointer type

/*
	@class addLine_action is an action to add a line to a sketch, given two points, a sketch and a document
	@parent : action
*/
class addEntity_action : public action {
private:
	entity_ptr mChildEnt, mParentEnt;
	bool mValid;
public:
	addEntity_action(entity_ptr child, entity_ptr parent);

	/*
		@function do_work activates the created line
	*/
	virtual bool do_work();
	/*
		@function undo_work desactivates the created line
	*/
	virtual bool undo_work();

	entity_ptr child_entity() const { return mChildEnt; }
	entity_ptr parent_entity() const { return mParentEnt; }
};

#endif