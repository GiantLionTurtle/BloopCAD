
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

#ifndef ENTERPARTDESIGN_ACTION_HPP_
#define ENTERPARTHDESIGN_ACTION_HPP_

#include <Actions/Common/SwitchWorkspace_action.hpp>

#include <memory>

class Part;

/*
	@class EnterPart_action enters the Part workspace with a Part target

	@parent SwitchWorkspace_action
	@note Might be concatenated with QuitPart_action as the origin visibility logic is duplicated
*/
class EnterPart_action : public SwitchWorkspace_action {
private:
	Part* mTarget; // Target of the workspace
public:
	/*
		@constructor EnterPart_action

		@param set_camera	If the camera of the new workspace should be set to be at
							the same state then the camera of the workspace that is left
	*/
	EnterPart_action(bool set_camera = false);

	/*
		@function do_work switches to Part workspace

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work revert back to the initial workspace

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);
};

#endif