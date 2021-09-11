
#ifndef SWITCHWORKSPACE_ACTION_HPP_
#define SWITCHWORKSPACE_ACTION_HPP_

#include <Actions/Action.hpp>

#include <memory>

/*
	@class SwitchWorkspace_action is an action to change the workspace of a Document
	@parent : action
*/
class SwitchWorkspace_action : public Action {
private:
	int mInitWorkspaceName, mTargetWorkspaceName; // The workspace it was in initialy, and the workspace it will be in after
	bool mValid; // If the action is valid
	bool mSet_Camera; 
public:
	/*
		@function SwitchWorkspace_action creates the action and saves the current workspace

		@param doc : 			The Document onto which to perform the action
		@param workspaceNme : 	The name of the target workspace
	*/
	SwitchWorkspace_action(int workspaceName, bool set_Camera = false);

	/*
		@function do_work switches to desired workspace if it exists
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work revert back to the initial workspace
	*/
	virtual bool undo_work(Document* caller);

	void switch_workspace(Document* caller, int wrksp);
};

#endif