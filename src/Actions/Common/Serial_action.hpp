
#ifndef SERIAL_ACTION_HPP_
#define SERIAL_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Drawables/Base/Drawable.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Serial_action : public Action {
private:
	std::vector<Action_ptr> mActions;
	int mCurrentInd;
public:
	Serial_action(std::vector<Action_ptr> actions);
	Serial_action();

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);

	void add_action(Action_ptr anAction);

	int size() const { return mActions.size(); }
	int ind() const { return mCurrentInd; }
};

#endif