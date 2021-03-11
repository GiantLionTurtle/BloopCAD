
#include "serial_action.hpp"

serial_action::serial_action(std::vector<std::shared_ptr<action>> actions):
	mActions(actions),
	mCurrentInd(0)
{

}
serial_action::serial_action():
	mActions({}),
	mCurrentInd(0)
{

}


bool serial_action::do_work(document* caller)
{
	if(mCurrentInd >= 0 && mCurrentInd < mActions.size() && mActions[mCurrentInd]->do_work(caller)) {
		mCurrentInd++;
	}
	return mCurrentInd >= mActions.size();
}
bool serial_action::undo_work(document* caller)
{
	if(mCurrentInd > 0 && mCurrentInd <= mActions.size() && mActions[mCurrentInd-1]->undo_work(caller)) {
		mCurrentInd--;
	}
	return mCurrentInd <= 0;
}

void serial_action::add_action(std::shared_ptr<action> anAction)
{
	mActions.push_back(anAction);
}