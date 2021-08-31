
#include "SwitchWorkspace_action.hpp"
#include <utils/DebugUtils.hpp>
#include <document.hpp>

SwitchWorkspace_action::SwitchWorkspace_action(int workspaceName, bool set_Camera):
	mTargetWorkspaceName(workspaceName),
	mSet_Camera(set_Camera)
{

}

bool SwitchWorkspace_action::do_work(document* caller)
{
	if(caller) {
		if(caller->state()) {
			mInitWorkspaceName = caller->state()->workspaceName; // save the document's current workspace
			mValid = caller->has_workspace(mTargetWorkspaceName); // Check if the target workspace exists within the target document
		} else {
			LOG_WARNING("No workspace state.");
		}
	} else {
		mValid = false;
		LOG_WARNING("Document pointer is not valid.");
	}
	Camera_ptr tmpCam;
	if(mSet_Camera)
		tmpCam = caller->state()->cam;
	CameraState startState = caller->state()->cam->state();
	switch_workspace(caller, mTargetWorkspaceName);
	caller->state()->startCamState = startState;
	if(mSet_Camera)
		caller->state()->cam->copy(tmpCam);
	return true;
}
bool SwitchWorkspace_action::undo_work(document* caller)
{
	switch_workspace(caller, mInitWorkspaceName);
	return true;
}

void SwitchWorkspace_action::switch_workspace(document* caller, int name)
{
	if(mValid) {
		workspace_ptr space = caller->set_workspace(name);
		if(space) 
			space->set_tool(space->defaultTool());
	}
}