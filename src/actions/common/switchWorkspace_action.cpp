
#include "switchWorkspace_action.hpp"
#include <utils/errorLogger.hpp>
#include <document.hpp>

switchWorkspace_action::switchWorkspace_action(int workspaceName, bool set_camera):
	mTargetWorkspaceName(workspaceName),
	mSet_camera(set_camera)
{

}

bool switchWorkspace_action::do_work(document* caller)
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
	camera_ptr tmpCam;
	if(mSet_camera)
		tmpCam = caller->state()->cam;
	cameraState startState = caller->state()->cam->state();
	switch_workspace(caller, mTargetWorkspaceName);
	caller->state()->startCamState = startState;
	if(mSet_camera)
		caller->state()->cam->copy(tmpCam);
	return true;
}
bool switchWorkspace_action::undo_work(document* caller)
{
	switch_workspace(caller, mInitWorkspaceName);
	return true;
}

void switchWorkspace_action::switch_workspace(document* caller, int name)
{
	if(mValid) {
		workspace_ptr space = caller->set_workspace(name);
		if(space) 
			space->set_tool(space->defaultTool());
	}
}