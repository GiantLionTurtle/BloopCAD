
#include "switchWorkspace_action.hpp"
#include <utils/errorLogger.hpp>
#include <document.hpp>

switchWorkspace_action::switchWorkspace_action(std::string const& workspaceName, bool set_camera):
	mTargetWorkspaceName(workspaceName),
	mSet_camera(set_camera)
{

}

bool switchWorkspace_action::do_work(document* caller)
{
	if(caller) {
		if(caller->currentWorkspaceState()) {
			mInitWorkspaceName = caller->currentWorkspaceState()->workspaceName; // save the document's current workspace

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
		tmpCam = caller->currentWorkspaceState()->cam;
	camState startState = caller->currentWorkspaceState()->cam->state();
	switch_workspace(caller, mTargetWorkspaceName);
	caller->currentWorkspaceState()->startCamState = startState;
	if(mSet_camera)
		caller->currentWorkspaceState()->cam->set(tmpCam);
	return true;
}
bool switchWorkspace_action::undo_work(document* caller)
{
	switch_workspace(caller, mInitWorkspaceName);
	return true;
}

void switchWorkspace_action::switch_workspace(document* caller, std::string const& name)
{
	if(mValid) {
		workspace_ptr space = caller->set_workspace(name);
		if(space) 
			space->set_tool(space->defaultTool());
	}
}