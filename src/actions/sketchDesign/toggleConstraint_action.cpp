
#include "toggleConstraint_action.hpp"
#include <utils/DebugUtils.hpp>
#include <document.hpp>

toggleConstraint_action::toggleConstraint_action(Sketch* parent_sketch, SkConstraint* constr, bool toggle_to, bool done_at_creation):
	mSketch_parent(parent_sketch),
	mConstraint(constr),
	mToggle_to(toggle_to),
	mDone(done_at_creation)
{
	DEBUG_ASSERT(parent_sketch, "Invalid sketch.");
	DEBUG_ASSERT(constr, "Invalid constraint.");
}

bool toggleConstraint_action::do_work(document* caller)
{
	if(mDone)
		return true;
	mSketch_parent->toggle_constraint(mConstraint, mToggle_to);
	return true;
}
bool toggleConstraint_action::undo_work(document* caller)
{
	mDone = false;
	mSketch_parent->toggle_constraint(mConstraint, !mToggle_to);
	return true;
}