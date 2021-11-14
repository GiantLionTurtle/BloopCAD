
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