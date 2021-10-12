
#ifndef PARALLEL_ACTION_HPP_
#define PARALLEL_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/entity.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class parallel_action : public action {
private:
	std::vector<std::shared_ptr<action>> mActions;
    std::vector<bool> mActions_done;
public:
	parallel_action(std::vector<std::shared_ptr<action>> actions);
	parallel_action();

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);

	void add_action(std::shared_ptr<action> anAction);

	int size() const { return mActions.size(); }
};

#endif