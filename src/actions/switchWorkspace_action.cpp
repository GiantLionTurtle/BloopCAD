
#include "switchWorkspace_action.hpp"

switchWorkspace_action::switchWorkspace_action(document* doc, std::string const& workspaceName):
	mDoc(doc),
	mTargetWorkspaceName(workspaceName)
{

}

void switchWorkspace_action::do_work()
{
	if(mDoc) {
		mInitWorkspaceName = mDoc->currentWorkspaceState()->workspaceName;
		mDoc->set_workspace(mTargetWorkspaceName);
	} else {
		LOG_WARNING("Document pointer is not valid.");
	}
}
void switchWorkspace_action::undo_work()
{
	if(mDoc) {
		mDoc->set_workspace(mInitWorkspaceName);
	} else {
		LOG_WARNING("Document pointer is not valid.");
	}
}