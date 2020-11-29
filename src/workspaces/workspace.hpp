
#ifndef WORKSPACE_HPP_
#define WORKSPACE_HPP_

#include <forward_bloop.hpp>
#include <graphics_utils/camera.hpp>
#include <tools/tool.hpp>
#include <entities/entitiesIndexer.hpp>
#include <graphics_utils/frameBuffer.hpp>

#include <gtkmm.h>

#include <map>
#include <string>
#include <memory>

struct workspaceState {
	document* doc;
	std::shared_ptr<entity> target;
	std::shared_ptr<camera> cam;
	std::shared_ptr<tool_abstract> currentTool;
	std::string workspaceName;
};

class workspace {
protected:
	int id;
	static int count;
	std::map<unsigned int, std::string> mKey_invokes;
	std::map<unsigned int, std::string> mMouse_invokes;

	std::map<std::string, std::shared_ptr<tool_abstract>> mTools;
	std::shared_ptr<tool_abstract> mDefaultTool;
	std::shared_ptr<entity> mTarget;

	std::shared_ptr<workspaceState> mState;

	Gtk::Box* mUpperBar;

	bloop* mParentBloop;
public:
	workspace(bloop* parent);
	workspace(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	bool invoke_from_key(unsigned int key, std::string& toolName);

	void set_target(std::shared_ptr<entity> target) { mTarget = target; }

	bool set_tool(std::string const& name);
	void set_tool(std::shared_ptr<tool_abstract> tool_);
	void set_tool_cursor(std::shared_ptr<tool_abstract> tool_);
	std::shared_ptr<tool_abstract> defaultTool() { return mDefaultTool; }

	bool manage_key_press(GdkEventKey* event);
	bool manage_key_release(GdkEventKey* event);
	bool manage_mouse_move(GdkEventMotion* event);
	bool manage_mouse_scroll(GdkEventScroll* event);
	bool manage_button_press(GdkEventButton* event);
	bool manage_button_release(GdkEventButton* event);

	std::shared_ptr<workspaceState> state() { return mState; }
	void set_state(std::shared_ptr<workspaceState> state_);

	Gtk::Box* upperBar() { return mUpperBar; }

};

#endif