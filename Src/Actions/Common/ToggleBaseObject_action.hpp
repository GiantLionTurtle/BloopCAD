
#ifndef TOGGLEBASEOBJECT_ACTION_HPP_
#define TOGGLEBASEOBJECT_ACTION_HPP_

#include <Actions/Action.hpp>
#include <BaseObject.hpp>

class ToggleBaseObject_action : public Action {
private:
	BaseObject* mObj;
	bool mToggle_to;
public:
	ToggleBaseObject_action(BaseObject* obj, bool toggle_to);

	virtual bool do_work(Document* caller);
	virtual bool undo_work(Document* caller);
};

#endif