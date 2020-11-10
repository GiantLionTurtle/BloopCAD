
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

	void update_state_dims(); // TODO: find a better name

	bool set_workspace(std::string const& name);
	bool set_workspace();
	std::shared_ptr<workspaceState> currentWorkspaceState() { return mCurrentWorkspaceState; };
};

#endif