
#ifndef TOGGLEBASEOBJECT_ACTION_HPP_
#define TOGGLEBASEOBJECT_ACTION_HPP_

#include <actions/action.hpp>
#include <baseObject.hpp>

class toggleBaseObject_action : public action {
private:
	baseObject_ptr mObj;
	bool mToggle_to;
public:
	toggleBaseObject_action(baseObject_ptr obj, bool toggle_to);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif