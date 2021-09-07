
#include "ToggleConstraint_action.hpp"
#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>

ToggleConstraint_action::ToggleConstraint_action(Sketch* parent_sketch, SkConstraint* constr, bool toggle_to, bool done_at_creation):
	mSketch_parent(parent_sketch),
	mConstraint(constr),
	mToggle_to(toggle_to),
	mDone(done_at_creation)
{
	DEBUG_ASSERT(parent_sketch, "Invalid sketch.");
	DEBUG_ASSERT(constr, "Invalid constraint.");
}

bool ToggleConstraint_action::do_work(Document* caller)
{
	if(mDone)
		return true;
	mSketch_parent->toggle_constraint(mConstraint, mToggle_to);
	return true;
}
bool ToggleConstraint_action::undo_work(Document* caller)
{
	mDone = false;
	mSketch_parent->toggle_constraint(mConstraint, !mToggle_to);
	return true;
}