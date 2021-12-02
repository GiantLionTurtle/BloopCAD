
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

#ifndef SWITCHWORKSPACE_ACTION_HPP_
#define SWITCHWORKSPACE_ACTION_HPP_

#include <Actions/Action.hpp>

#include <memory>

/*
	@class SwitchWorkspace_action is an action to change the workspace of a Document

	@parent : action
*/
class SwitchWorkspace_action : public Action {
private:
	int mInitWorkspaceName, mTargetWorkspaceName; // The workspace it was in initialy, and the workspace it will be in after
	bool mValid; // If the action is valid
	bool mSet_Camera; 
public:
	/*
		@function SwitchWorkspace_action creates the action and saves the current workspace

		@param workspaceNme	The name of the target workspace
		@param set_camera	If the camera of the new workspace should be set to be at
							the same state then the camera of the workspace that is left
	*/
	SwitchWorkspace_action(int workspaceName, bool set_camera = false);

	/*
		@function do_work switches to desired workspace if it exists and clone the initial workespace's
		camera into the new workspace's camera if desired

		@param caller	The document calling the function and that will have it's workspace switched
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work revert back to the initial workspace and clone the target workespace's
		camera into the initial's camera if desired
	
		@param caller	The document calling the function and that will have it's workspace switched
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);

	/*
		@function switch_workspace switches the workspace to a target workspace
		
		@param 			The document onto which the workspace will act
		@param wrksp 	The name of the workspace (which is a number refering to an enum in Bloop)
		@note This is the core of the class
	*/
	void switch_workspace(Document* caller, int wrksp);
};

#endif