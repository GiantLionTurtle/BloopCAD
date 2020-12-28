
#ifndef SWITCHWORKSPACE_ACTION_HPP_
#define SWITCHWORKSPACE_ACTION_HPP_

#include <actions/action.hpp>

#include <memory>

class document; // Declaration of document needed to declare pointer types

/*
	@class switchWorkspace_action is an action to change the workspace of a document
	@parent : action
*/
class switchWorkspace_action : public action {
private:
	document* mDoc;	// The target document
	std::string mInitWorkspaceName, mTargetWorkspaceName; // The workspace it was in initialy, and the workspace it will be in after
	bool mValid; // If the action is valid
public:
	/*
		@function switchWorkspace_action creates the action and saves the current workspace

		@param doc : 			The document onto which to perform the action
		@param workspaceNme : 	The name of the target workspace
	*/
	switchWorkspace_action(document* doc, std::string const& workspaceName);

	/*
		@function do_work switches to desired workspace if it exists
	*/
	virtual bool do_work();
	/*
		@function undo_work revert back to the initial workspace
	*/
	virtual bool undo_work();
};

#endif