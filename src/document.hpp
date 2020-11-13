
#ifndef DOCUMENT_HPP_
#define DOCUMENT_HPP_

#include <forward_bloop.hpp>
#include <workspaces/workspace.hpp>
#include <entities/entity.hpp>
#include <entities/entitiesIndexer.hpp>
#include <entities/plane_abstract.hpp>
#include <tools/tool.hpp>
#include <graphics_utils/frameBuffer.hpp>
#include <actions/action.hpp>

#include <gtkmm.h>

#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <memory>

class document : public Gtk::Box {
private:
	std::shared_ptr<entitiesIndexer> mEntitiesIndexer;
	std::shared_ptr<entity> mSubject;

	std::map<std::string, std::shared_ptr<workspaceState>> mWorkspaceStates;
	std::shared_ptr<workspaceState> mCurrentWorkspaceState;

	std::shared_ptr<frameBuffer> mSelectionBuffer;

	std::vector<std::shared_ptr<action>> mActionStack;
	unsigned int mActionInd, mActionStackSize;

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
	bool set_workspace();
	std::shared_ptr<workspaceState> currentWorkspaceState() { return mCurrentWorkspaceState; };

	std::shared_ptr<entitiesIndexer> indexer() { return mEntitiesIndexer; }
	std::shared_ptr<frameBuffer> selectionBuffer() { return mSelectionBuffer; }

	void push_action(std::shared_ptr<action> to_push);
	void advance_action_index(unsigned int amount = 1);
	void rewind_action_index(unsigned int amount = 1);
};

#endif