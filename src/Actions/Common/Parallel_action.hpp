
#ifndef PARALLEL_ACTION_HPP_
#define PARALLEL_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Drawables/Base/Drawable.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Parallel_action : public Action {
private:
	std::vector<Action_ptr> mActions;
    std::vector<bool> mActions_done;
public:
	Parallel_action(std::vector<Action_ptr> actions);
	Parallel_action();

	virtual bool do_work(Document* caller);
	virtual bool undo_work(Document* caller);

	void add_action(Action_ptr anAction);

	int size() const { return mActions.size(); }
};

#endif