
#ifndef SWITCHWORKSPACE_ACTION_HPP_
#define SWITCHWORKSPACE_ACTION_HPP_

#include <actions/action.hpp>
#include <document.hpp>

#include <memory>

class switchWorkspace_action : public action {
private:
	document* mDoc;
	std::string mInitWorkspaceName, mTargetWorkspaceName;
public:
	switchWorkspace_action(document* doc, std::string const& workspaceName);

	virtual void do_work();
	virtual void undo_work();
};

#endif