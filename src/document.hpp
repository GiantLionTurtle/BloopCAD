
#ifndef DOCUMENT_HPP_
#define DOCUMENT_HPP_

#include <forward_bloop.hpp>
#include <workspaces/workspace.hpp>
#include <entities/forward_entities.hpp>
#include <tools/tool.hpp>
#include <graphics_utils/frameBuffer.hpp>
#include <actions/action.hpp>
#include <entities/part.hpp>
#include <graphics_utils/camera.hpp>

#include <gtkmm.h>

#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <memory>

struct selection {
	std::shared_ptr<entity> ent;
	camState cam;

	selection(std::shared_ptr<entity> ent_, camState cam_) :
		ent(ent_),
		cam(cam_)
	{}
};

class document : public Gtk::Box {
private:
	std::shared_ptr<part> mPart;
	std::vector<std::pair<glm::ivec3, selection>> mSelection; // This will be kept linear for as long as there is no need for large selections

	std::map<std::string, std::shared_ptr<workspaceState>> mWorkspaceStates;
	std::shared_ptr<workspaceState> mCurrentWorkspaceState;

	std::shared_ptr<frameBuffer> mSelectionBuffer;

	std::vector<std::shared_ptr<action>> mActionStack;
	unsigned int mActionInd, mActionStackSize;

	glm::vec3 mBackgroundColor;
	Gtk::GLArea mViewport;
	bloop* mParentBloop;
public:
	document(bloop* parent);

	void make_glContext_current();

	void do_realize();
	void do_unrealize();
	bool do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */);
	static gboolean frame_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer data);
	void connect_signals();

	bool set_workspace(std::string const& name);
	bool set_workspace();
	std::shared_ptr<workspaceState> currentWorkspaceState() { return mCurrentWorkspaceState; };

	std::shared_ptr<part> target() { return mPart; }
	std::shared_ptr<entitiesIndexer> indexer() { return mPart; }
	std::shared_ptr<frameBuffer> selectionBuffer() { return mSelectionBuffer; }

	void push_action(std::shared_ptr<action> to_push);
	void advance_action_index(unsigned int amount = 1);
	void rewind_action_index(unsigned int amount = 1);

	unsigned int selection_size() const { return mSelection.size(); }
	selection selection_at(unsigned int ind);
	int selection_ind(glm::ivec3 id);
	void clear_selection();
	void toggle_select(glm::ivec3 id, camState cam, bool additive);
};

#endif