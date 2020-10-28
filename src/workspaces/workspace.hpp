
#ifndef WORKSPACE_HPP_
#define WORKSPACE_HPP_

#include "../forward_bloop.hpp"
#include "../errorLogger.hpp"
#include "../graphics_utils/camera.hpp"
#include "../tools/tool.hpp"
#include "../entities/entitiesIndexer.hpp"
#include "../graphics_utils/frameBuffer.hpp"

#include <gtkmm.h>

#include <map>
#include <string>
#include <memory>

struct workspaceState {
	int width, height;
	std::shared_ptr<entitiesIndexer> indexer;
	std::shared_ptr<frameBuffer> selectionBuffer;
	std::shared_ptr<camera> cam;
	std::shared_ptr<tool_abstract> currentTool;
};

class workspace {
protected:
	int id;
	static int count;
	std::map<unsigned int, std::string> mKey_invokes;
	std::map<unsigned int, std::string> mMouse_invokes;

	std::map<std::string, std::shared_ptr<tool_abstract>> mTools;
	std::shared_ptr<entity> mTarget;

	std::shared_ptr<std::shared_ptr<workspaceState>> mState;

	Gtk::Box* mUpperBar;

	bloop* mParentBloop;
public:
	workspace(bloop* parent);
	workspace(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	bool invoke_from_key(unsigned int key, std::string& toolName);

	template<typename actOn>
	bool set_tool_target(std::string const& toolName)
	{
		if(mTools.find(toolName) != mTools.end() && mTools.at(toolName)) {
			try {
				std::dynamic_pointer_cast<tool<actOn>>(mTools.at(toolName))->set_target(std::dynamic_pointer_cast<actOn>(mTarget));
			} catch (std::exception& ex) {
				LOG_ERROR("Failed to set target for tool " + toolName + ": " + ex.what());
				return false;
			}
			return true;
		}
		return false;
	}

	void set_target(std::shared_ptr<entity> target) { mTarget = target; }

	bool set_tool(std::string const& name);

	bool manage_key_press(GdkEventKey* event);
	bool manage_mouse_move(GdkEventMotion* event);
	bool manage_mouse_scroll(GdkEventScroll* event);
	bool manage_button_release(GdkEventButton* event);

	std::shared_ptr<workspaceState> state() const { return (*mState); }
	void set_state(std::shared_ptr<workspaceState> state_) 
	{ 
		if(mState) {
			(*mState) = state_; 
		} else {
			LOG_ERROR("No state available.");
		}
	}

	Gtk::Box* upperBar() { return mUpperBar; }
};

#endif