
#ifndef APPLYSNAPSHOT_ACTION_HPP_
#define APPLYSNAPSHOT_ACTION_HPP_

#include <actions/action.hpp>
#include <constraintsSolver/expression.hpp>

class applySnapshot_action : public action {
private:
    std::vector<VarDualState> mDeltas;
    bool mDone;
public:
	applySnapshot_action(std::vector<VarDualState> deltas, bool done_at_creation);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif