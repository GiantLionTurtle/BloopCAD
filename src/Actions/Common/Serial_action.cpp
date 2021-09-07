
#include "Serial_action.hpp"

Serial_action::Serial_action(std::vector<Action_ptr> actions):
	mActions(actions),
	mCurrentInd(0)
{

}
Serial_action::Serial_action():
	mActions({}),
	mCurrentInd(0)
{

}


bool Serial_action::do_work(Document* caller)
{
	if(mCurrentInd >= 0 && mCurrentInd < mActions.size() && mActions[mCurrentInd]->do_work(caller)) {
		mCurrentInd++;
	}
	return mCurrentInd >= mActions.size();
}
bool Serial_action::undo_work(Document* caller)
{
	if(mCurrentInd > 0 && mCurrentInd <= mActions.size() && mActions[mCurrentInd-1]->undo_work(caller)) {
		mCurrentInd--;
	}
	return mCurrentInd <= 0;
}

void Serial_action::add_action(Action_ptr anAction)
{
	mActions.push_back(anAction);
}