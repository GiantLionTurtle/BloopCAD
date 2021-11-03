
#include "ApplySnapshot_action.hpp"

#include <Drawables/Containers/Sketch.hpp>

ApplySnapshot_action::ApplySnapshot_action(Sketch* sk, std::vector<ParamDualState> deltas, bool done_at_creation):
	mDeltas(deltas),
	mDone(done_at_creation),
	mSketch(sk)
{

}

bool ApplySnapshot_action::do_work(Document* caller)
{
	if(mDone) // Will skip the first do_work, but will not impeed the others after an undo_work
		return true;
	// mSketch->apply_deltaSnapshot(mDeltas, true); // Apply first set of values
	return true;
}
bool ApplySnapshot_action::undo_work(Document* caller)
{
	mDone = false; // This will never be true again
	// mSketch->apply_deltaSnapshot(mDeltas, false); // Apply second set of values
	return true;
}
