
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

#ifndef TOGGLEBASEOBJECT_ACTION_HPP_
#define TOGGLEBASEOBJECT_ACTION_HPP_

#include <Actions/Action.hpp>

class BaseObject;

/*
	@class ToggleBaseObject_action toggles the existence of an object
	it is widely used, every time a Drawable is added and has to be able
	to be deleted

	@parent Action
*/
class ToggleBaseObject_action : public Action {
private:
	BaseObject* mObj; // Object to toggle
	bool mToggle_to; // The first state to toggle it to un "do", if something is added, it will be True
public:
	/*
		@constructor ToggleBaseObject_action

		@param obj 			The object to manipulate
		@param toggle_to 	Target initial state
	*/
	ToggleBaseObject_action(BaseObject* obj, bool toggle_to);

	/*
		@function do_work toggles the object existence to the target state

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work toggles the object existence to the inverse of the target state

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);
};

#endif