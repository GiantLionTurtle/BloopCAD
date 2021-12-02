
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Serial_action.hpp"

Serial_action::Serial_action(std::vector<Action_ptr> actions)
	: mActions(actions)
	, mCurrentInd(0) // Actions are assumed not done, so the execution id is 0
{

}
Serial_action::Serial_action()
	: mActions({})
	, mCurrentInd(0) // Actions are assumed not done, so the execution id is 0
{

}


bool Serial_action::do_work(Document* caller)
{
	if(mCurrentInd >= 0 && mCurrentInd < mActions.size() && mActions[mCurrentInd]->do_work(caller)) {
		mCurrentInd++; // Increment execution id if the work is done on the current action
	}
	return mCurrentInd == mActions.size(); // Done if the last action is done
}
bool Serial_action::undo_work(Document* caller)
{
	// the execution id is mCurrentInd - 1 here to not have to decrement the index 
	// on the last "do" call or the first "undo" call, it's just cleaner
	if(mCurrentInd > 0 && mCurrentInd <= mActions.size() && mActions[mCurrentInd-1]->undo_work(caller)) {
		mCurrentInd--; // Decrement execution id if the work is undone on the current action
	}
	return mCurrentInd == 0; // All actions are undone;
}

void Serial_action::add_action(Action_ptr anAction)
{
	mActions.push_back(anAction);
}