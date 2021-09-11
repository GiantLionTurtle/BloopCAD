
#include "ApplySnapshot_action.hpp"

#include <Drawables/Containers/Sketch.hpp>

ApplySnapshot_action::ApplySnapshot_action(std::vector<VarDualState> deltas, bool done_at_creation):
	mDeltas(deltas),
	mDone(done_at_creation)
{

}

bool ApplySnapshot_action::do_work(Document* caller)
{
	if(mDone)
		return true;
	Sketch::apply_deltaSnapshot(mDeltas, true);
	return true;
}
bool ApplySnapshot_action::undo_work(Document* caller)
{
	mDone = false;
	Sketch::apply_deltaSnapshot(mDeltas, false);
	return true;
}
