
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

#include "SwitchWorkspace_action.hpp"
#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>

SwitchWorkspace_action::SwitchWorkspace_action(int workspaceName, bool set_camera)
	: mTargetWorkspaceName(workspaceName)
	, mSet_Camera(set_camera)
{

}

bool SwitchWorkspace_action::do_work(Document* caller)
{
	mInitWorkspaceName = caller->state()->workspaceName; // save the Document's current workspace
	mValid = caller->has_workspace(mTargetWorkspaceName); // Check if the target workspace exists within the target Document

	Camera* tmpCam;
	CameraState startState;
	if(mSet_Camera) {
		tmpCam = caller->state()->cam; // Remember the camera from the initial workspace to clone it back into the target state
		startState = caller->state()->cam->state(); // Remember the state that the camera started in for transitioning out of this workspace
	}
	switch_workspace(caller, mTargetWorkspaceName); // Do the switching
	if(mSet_Camera) {
		caller->state()->cam->copy(tmpCam); // Copy inital workspace's camera into target state
		caller->state()->startCamState = startState; // Copy initial workspace
	}
	return true;
}
bool SwitchWorkspace_action::undo_work(Document* caller)
{
	// Camera is not set the same way it was because it is assumed to be handled trough camera move, it might change
	switch_workspace(caller, mInitWorkspaceName); // Switch
	return true;
}

void SwitchWorkspace_action::switch_workspace(Document* caller, int name)
{
	DEBUG_ASSERT(caller, "Document pointer is not valid.");
	DEBUG_ASSERT(caller->state(), "No workspace state.");

	Workspace_abstr* space = caller->set_workspace(name); // Set the workspace
	if(space) // Set the workspace tool
		space->set_tool(space->defaultTool());
}