
#include "toggleConstraint_action.hpp"
#include <utils/errorLogger.hpp>
#include <document.hpp>

toggleConstraint_action::toggleConstraint_action(sketch_ptr parent_sketch, std::shared_ptr<constraint_entity> constr, bool toggle_to):
	mSketch_parent(parent_sketch),
	mConstraint(constr),
	mToggle_to(toggle_to)
{
	DEBUG_ASSERT(parent_sketch, "Invalid sketch.");
	DEBUG_ASSERT(constr, "Invalid constraint.");
}

bool toggleConstraint_action::do_work(document* caller)
{
	mSketch_parent->toggle_constraint(mConstraint, mToggle_to);
	return true;
}
bool toggleConstraint_action::undo_work(document* caller)
{
	mSketch_parent->toggle_constraint(mConstraint, !mToggle_to);
	return true;
}