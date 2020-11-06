
#ifndef DOCUMENT_HPP_
#define DOCUMENT_HPP_

#include "forward_bloop.hpp"
#include "workspaces/workspace.hpp"
#include "entities/entity.hpp"
#include "entities/entitiesIndexer.hpp"
#include "entities/plane_abstract.hpp"
#include "tools/tool.hpp"
#include "graphics_utils/frameBuffer.hpp"

#include <gtkmm.h>

#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <memory>

class document : public Gtk::Box {
private:
	std::shared_ptr<entitiesIndexer> mEntities;
	std::shared_ptr<entity> mSubject;

	std::map<std::string, std::shared_ptr<workspaceState>> mWorkspaceStates;
	std::shared_ptr<workspaceState> mCurrentWorkspaceState;

	std::shared_ptr<frameBuffer> mSelectionBuffer;

	Gtk::GLArea mViewPort;

	bloop* mParentBloop;
public:
	document(bloop* parent);

	void do_realize();
	void do_unrealize();
	bool do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */);
	static gboolean frame_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer data);
	void connect_signals();

	bool set_workspace(std::string const& name);
	std::shared_ptr<workspaceState> currentWorkspaceState() { return mCurrentWorkspaceState; };

	// void set_tool(std::shared_ptr<tool_abstract> aTool) { if(mCurrentWorkspaceState->currentTool) mCurrentWorkspaceState->currentTool->finish(); mCurrentWorkspaceState->currentTool = aTool; }

	bool manage_key_press(GdkEventKey* event){ if(mCurrentWorkspaceState->currentTool) return mCurrentWorkspaceState->currentTool->manage_key_press(event); return false; };
	bool manage_mouse_scroll(GdkEventScroll* event);
	bool manage_mouse_move(GdkEventMotion* event);


	glm::ivec3 selection_buffer_at(glm::ivec2 pos, bool& success);
};

#endif