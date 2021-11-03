
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#ifndef APPLYSNAPSHOT_ACTION_HPP_
#define APPLYSNAPSHOT_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Utils/Param.hpp>

#include <vector>

class Sketch;

/*
	@class ApplySnapshot_action: Sets a group of var_ptr to
	one of two specified values

	@from
*/
class ApplySnapshot_action : public Action {
private:
	std::vector<ParamDualState> mDeltas; // Group of variables with their two possible values
	bool mDone; // If the action is done or undone (useful if the variables are already at the done
				// state when the action is created)
	Sketch* mSketch; // The sketch that will be updated after setting the variables
public:
	/*
		@constructor AppluSnapshot_action

		@param sk 				The sketch that contains the variable(but not forced to ) and will be updated after set
		@param deltas			List of all involved var_ptr with their two states
		@param done_at_creation	If the var_ptr are already set to the "done" state
	*/
	ApplySnapshot_action(Sketch* sk, std::vector<ParamDualState> deltas, bool done_at_creation);

	/*
		@function do_work sets the var_ptr to their first state

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function do_work sets the var_ptr to their second state

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);
};

#endif