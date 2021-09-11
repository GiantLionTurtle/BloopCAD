
#include "Parallel_action.hpp"
#include <algorithm>

Parallel_action::Parallel_action(std::vector<Action_ptr> actions):
	mActions(actions),
	mActions_done(actions.size(), false)
{

}
Parallel_action::Parallel_action():
	mActions({}),
	mActions_done({})
{

}


bool Parallel_action::do_work(Document* caller)
{
	int n_pendingActions = 0;
	for(int i = 0; i < mActions.size() && i < mActions_done.size(); ++i) {
		if(!mActions_done[i]) {
			n_pendingActions++;
			if(mActions[i]->do_work(caller)) {
				mActions_done[i] = true;
				n_pendingActions--;
			}
		}
	}
	return n_pendingActions == 0;
}
bool Parallel_action::undo_work(Document* caller)
{
	int n_pendingActions = 0;
	for(int i = mActions.size(); i >= 0; --i) {
		if(mActions_done[i]) {
			n_pendingActions++;
			if(mActions[i]->undo_work(caller)) {
				mActions_done[i] = false;
				n_pendingActions--;
			}
		}
	}
	return n_pendingActions == 0;
}

void Parallel_action::add_action(Action_ptr anAction)
{
	mActions.push_back(anAction);
	mActions_done.push_back(false);
}