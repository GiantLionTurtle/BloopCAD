
#ifndef SWITCHWORKSPACE_ACTION_HPP_
#define SWITCHWORKSPACE_ACTION_HPP_

#include <actions/action.hpp>

#include <memory>

/*
	@class switchWorkspace_action is an action to change the workspace of a document
	@parent : action
*/
class switchWorkspace_action : public action {
private:
	int mInitWorkspaceName, mTargetWorkspaceName; // The workspace it was in initialy, and the workspace it will be in after
	bool mValid; // If the action is valid
	bool mSet_Camera; 
public:
	/*
		@function switchWorkspace_action creates the action and saves the current workspace

		@param doc : 			The document onto which to perform the action
		@param workspaceNme : 	The name of the target workspace
	*/
	switchWorkspace_action(int workspaceName, bool set_Camera = false);

	/*
		@function do_work switches to desired workspace if it exists
	*/
	virtual bool do_work(document* caller);
	/*
		@function undo_work revert back to the initial workspace
	*/
	virtual bool undo_work(document* caller);

	void switch_workspace(document* caller, int wrksp);
};

#endif