
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

#ifndef ENTERSKETCHDESIGN_ACTION_HPP_
#define ENTERSKETCHDESIGN_ACTION_HPP_

#include <Actions/Common/SwitchWorkspace_action.hpp>

#include <memory>

class Sketch;

/*
	@class EnterSketch_action: Allows to enter in sketch the Sketch workspace,
	set the workspace target to a specific sketche and set the camera to face the sketch

	@parent SwitchWorkspace_action
*/
class EnterSketch_action : public SwitchWorkspace_action {
private:
	Sketch* mTarget; // The sketch that will be edited once in Sketch workspace
public:
	/*
		@constructor EnterSketch_action

		@param target		The sketch to enter/edit
		@param set_camera	If the camera of the new workspace should be set to be at
							the same state then the camera of the workspace that is left
	*/
	EnterSketch_action(Sketch* target, bool set_camera = false);

	/*
		@function do_work switches to Sketch workspace (mainly uses SwitchWorkspace_action's function)

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);

	/*
		Note: undo_work is not overrided from SwitchWorkspace_action
	*/
};

#endif