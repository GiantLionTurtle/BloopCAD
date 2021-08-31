
#ifndef TOGGLEBASEOBJECT_ACTION_HPP_
#define TOGGLEBASEOBJECT_ACTION_HPP_

#include <Actions/Action.hpp>
#include <baseObject.hpp>

class ToggleBaseObject_action : public Action {
private:
	baseObject* mObj;
	bool mToggle_to;
public:
	ToggleBaseObject_action(baseObject* obj, bool toggle_to);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif