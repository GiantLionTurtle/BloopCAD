
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

#ifndef TOGGLEBASECONSTRAINT_ACTION_HPP_
#define TOGGLEBASECONSTRAINT_ACTION_HPP_

#include <Actions/Action.hpp>

#include <Drawables/Containers/Sketch.hpp>
#include <Drawables/Sk/SkConstraint.hpp>

/*
	@class ToggleConstraint_action enables and disables a constraint and ensure constraint system
	stays valid

	@parent Action
*/
class ToggleConstraint_action : public Action {
private:
	Sketch* mSketch_parent;     // Sketch that contains the constraint system (which contains the constraint)
	SkConstraint* mConstraint;  // Constraint to toggle
	bool mToggle_to, mDone;     // State it is in when the Action is created and if it is already in the right state at creation
public:
	/*
		@constructor ToggleConstraint_action

		@param parent_sketch Sketch that ultimatly contains the constraint
		@param constr           Constraint to toggle
		@param toggle_to        State to toggle to on do (True: exists, False: doesn't exist)
	*/
	ToggleConstraint_action(Sketch* parent_sketch, SkConstraint* constr, bool toggle_to, bool done_at_creation);

	/*
		@function do_work toggles the constraint to mToggle_to state and 
		solves the system
		
		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work toggles the constraint to !mToggle_to state and 
		solves the system
		
		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);
};

#endif