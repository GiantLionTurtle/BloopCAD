
#include "parallel_action.hpp"
#include <algorithm>

parallel_action::parallel_action(std::vector<std::shared_ptr<action>> actions):
	mActions(actions),
	mActions_done(actions.size(), false)
{

}
parallel_action::parallel_action():
	mActions({}),
	mActions_done({})
{

}


bool parallel_action::do_work(document* caller)
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
bool parallel_action::undo_work(document* caller)
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

void parallel_action::add_action(std::shared_ptr<action> anAction)
{
	mActions.push_back(anAction);
	mActions_done.push_back(false);
}