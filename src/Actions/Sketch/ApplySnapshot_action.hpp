
#ifndef APPLYSNAPSHOT_ACTION_HPP_
#define APPLYSNAPSHOT_ACTION_HPP_

#include <Actions/Action.hpp>
#include <ConstraintsSolver/Expression.hpp>

class ApplySnapshot_action : public Action {
private:
	std::vector<VarDualState> mDeltas;
	bool mDone;
public:
	ApplySnapshot_action(std::vector<VarDualState> deltas, bool done_at_creation);

	virtual bool do_work(Document* caller);
	virtual bool undo_work(Document* caller);
};

#endif