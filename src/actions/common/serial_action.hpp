
#ifndef SERIAL_ACTION_HPP_
#define SERIAL_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/entity.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class serial_action : public action {
private:
	std::vector<std::shared_ptr<action>> mActions;
	int mCurrentInd;
public:
	serial_action(std::vector<std::shared_ptr<action>> actions);
	serial_action();

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);

	void add_action(std::shared_ptr<action> anAction);

	int size() const { return mActions.size(); }
	int ind() const { return mCurrentInd; }
};

#endif