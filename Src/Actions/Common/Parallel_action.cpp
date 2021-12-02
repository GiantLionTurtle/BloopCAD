
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

#include "Parallel_action.hpp"
#include <algorithm>

Parallel_action::Parallel_action(std::vector<Action_ptr> actions)
	: mActions(actions)
	, mActions_done(actions.size(), false) // No action is done at this point
{

}
Parallel_action::Parallel_action()
	: mActions({})
	, mActions_done({})
{

}


bool Parallel_action::do_work(Document* caller)
{
	int n_pendingActions = 0; // number of actions that still need ticks to be done
	for(int i = 0; i < mActions.size(); ++i) {
		if(mActions_done[i]) // If the action is done ; skip
			continue;
		n_pendingActions++; // Add an action that need tick (if it's the last tick, it will be canceled)
		if(mActions[i]->do_work(caller)) {
			mActions_done[i] = true; // Remember that this action is done
			n_pendingActions--; // Cancel this pending action
		}
	}
	return n_pendingActions == 0;
}
bool Parallel_action::undo_work(Document* caller)
{
	// Same concept then do_work but reversed.
	int n_pendingActions = 0;
	for(int i = mActions.size(); i >= 0; --i) {
		if(!mActions_done[i]) 
			continue;
		n_pendingActions++;
		if(mActions[i]->undo_work(caller)) {
			mActions_done[i] = false;
			n_pendingActions--;
		}
	}
	return n_pendingActions == 0;
}

void Parallel_action::add_action(Action_ptr anAction)
{
	mActions.push_back(anAction);
	mActions_done.push_back(false); // Keep both vector of same length, action is assumed to not be done on add
}