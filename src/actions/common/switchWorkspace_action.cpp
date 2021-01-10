
#include "switchWorkspace_action.hpp"
#include <utils/errorLogger.hpp>
#include <document.hpp>

switchWorkspace_action::switchWorkspace_action(document* doc, std::string const& workspaceName):
	mDoc(doc),
	mTargetWorkspaceName(workspaceName)
{
	if(mDoc) {
		if(mDoc->currentWorkspaceState()) {
			mInitWorkspaceName = mDoc->currentWorkspaceState()->workspaceName; // save the document's current workspace

			mValid = mDoc->has_workspace(workspaceName); // Check if the target workspace exists within the target document
		} else {
			LOG_WARNING("No workspace state.");
		}
	} else {
		mValid = false;
		LOG_WARNING("Document pointer is not valid.");
	}
}

bool switchWorkspace_action::do_work()
{
	if(mValid) {
		workspace_ptr space = mDoc->set_workspace(mTargetWorkspaceName);
		if(space) 
			space->set_tool(space->defaultTool());
	}
	return true;
}
bool switchWorkspace_action::undo_work()
{
	if(mValid) {
		workspace_ptr space = mDoc->set_workspace(mInitWorkspaceName);
		if(space) 
			space->set_tool(space->defaultTool());
	}
	return true;
}