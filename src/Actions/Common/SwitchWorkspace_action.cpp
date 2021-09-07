
#include "SwitchWorkspace_action.hpp"
#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>

SwitchWorkspace_action::SwitchWorkspace_action(int workspaceName, bool set_Camera):
	mTargetWorkspaceName(workspaceName),
	mSet_Camera(set_Camera)
{

}

bool SwitchWorkspace_action::do_work(Document* caller)
{
	if(caller) {
		if(caller->state()) {
			mInitWorkspaceName = caller->state()->workspaceName; // save the Document's current workspace
			mValid = caller->has_workspace(mTargetWorkspaceName); // Check if the target workspace exists within the target Document
		} else {
			LOG_WARNING("No workspace state.");
		}
	} else {
		mValid = false;
		LOG_WARNING("Document pointer is not valid.");
	}
	Camera* tmpCam;
	if(mSet_Camera)
		tmpCam = caller->state()->cam;
	CameraState startState = caller->state()->cam->state();
	switch_workspace(caller, mTargetWorkspaceName);
	caller->state()->startCamState = startState;
	if(mSet_Camera)
		caller->state()->cam->copy(tmpCam);
	return true;
}
bool SwitchWorkspace_action::undo_work(Document* caller)
{
	switch_workspace(caller, mInitWorkspaceName);
	return true;
}

void SwitchWorkspace_action::switch_workspace(Document* caller, int name)
{
	if(mValid) {
		Workspace_abstr* space = caller->set_workspace(name);
		if(space) 
			space->set_tool(space->defaultTool());
	}
}