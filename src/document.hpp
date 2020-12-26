
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

/*
	@struct selection describes a selected entity from a certain camera viewpoint
*/
struct selection {
	std::shared_ptr<entity> ent; // The selected entity
	camState camSt; // The camera state at the moment of the selection

	/*
		@function selection creates a selection object

		@param ent_ : 	The selected entity
		@param camSt_ : The camera state during selection
	*/
	selection(std::shared_ptr<entity> ent_, camState camSt_) :
		ent(ent_),
		camSt(camSt_)
	{}
};

class document;
class entityView;

class entityHandle : public Gtk::EventBox {
private:
	Gtk::Box mBody;
	Gtk::Label mCollapser, mContent;
	Gtk::EventBox mCollapserEvents;

	std::shared_ptr<entity> mEntity;
	entityHandle* mParent;
	std::vector<entityHandle*> mChildren;
	entityView* mView;

	bool mCollapsed;
	int mLevel;
	bool mSelected;
public:
	entityHandle(std::shared_ptr<entity> ent, entityView* view, entityHandle* parent);
	entityHandle(); // for root handles

	bool set_hover(GdkEventCrossing* event);
	bool unset_hover(GdkEventCrossing* event);

	bool manage_collapse(GdkEventButton* event);
	bool select(GdkEventButton* event);

	int count_upTo(entityHandle* child);
	int count_all();

	entityHandle* parent() const { return mParent; }
	void set_parent(entityHandle* parent_) { mParent = parent_; }

	entityView* view() { return mView; };
	void set_view(entityView* view_) { mView = view_;}

	void set_selected(bool selected);

	// void add_child(entityHandle* handle);
};

class entityView : public Gtk::Box {
private:
	Gtk::Button btn;
	entityHandle mRootHandle;
	std::vector<entityHandle*> mHandles;
	document* mDoc;
public:
	entityView(document* doc);
	~entityView();

	void add_handle(entityHandle* handle, int at);

	entityHandle& root() { return mRootHandle; };
	document* doc() { return mDoc; }
};

/*
	@class document represents a document that the user is working with
	@parent Gtk::Box
*/
class document : public Gtk::Box {
private:
	std::shared_ptr<part> mPart; // Curently, documents only work on a single part
	std::vector<std::pair<glm::ivec3, selection>> mSelection; // This will be kept linear for as long as there is no need for large selections

	std::map<std::string, std::shared_ptr<workspaceState>> mWorkspaceStates; // All the workspace states for the various workspaces
	std::shared_ptr<workspaceState> mCurrentWorkspaceState; // The workspace state for the current workspace working on the document

	std::shared_ptr<frameBuffer> mSelectionBuffer; // The buffer onto which render the selection colors

	std::vector<std::shared_ptr<action>> mActionStack; // The stack of actions done in the document
	unsigned int mActionInd, mActionStackSize; // Metrics for the action stack

	glm::vec3 mBackgroundColor; // The rendering background color
	Gtk::GLArea mViewport; // The rendering widget
	int mFrameId; // The current frame id, it is NOT garanteed to be unique
	bloop* mParentBloop; // The bloop window owning the document
	camState mCurrentCamState;


	entityView* mSideBar;
public:
	/*
		@function document creates an empty and mostly unitialized document 

		@param parent : The bloop window owning the widget
	*/
	document(bloop* parent);

	/*
		@function make_glContext makes the rendering context current, it is used if the openGL states 
		have to be accessed from outside the rendering loop (like in a callback for instance)
	*/
	void make_glContext_current();

	/*
		@function do_realize initialize the docuement and realizes the viewport
	*/
	void do_realize();
	/*
		@function do_unrealize unrealizes the viewport
	*/
	void do_unrealize();
	/*
		@function do_render is the main rendering loop, entities draw themselves
		and their selection shadows in this function
	*/
	bool do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */);
	/*
		@function frame_callback calls the rendering loop on at a fixed interval
	*/
	static gboolean frame_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer data);
	
	void on_treeview_renderCell(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter);
	void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	
	/*
		@function connect_signals connects the viewport events to bloop's callbacks
	*/
	void connect_signals();

	entityView* sideBar() const { return mSideBar; }

	/*
		@function set_workspace sets the current workspace to a named workspace

		@param name : The name of the wanted workspace

		@return : The set workspace, or nullptr 
	*/
	std::shared_ptr<workspace> set_workspace(std::string const& name);
	/*
		@function set_workspace enforces the current workspace in the bloop window

		@return : The set workspace, or nullptr 
	*/
	std::shared_ptr<workspace> set_workspace();
	/*
		@function has_workspace tells if a named workspace is available

		@param name : The name of the wanted workspace

		@return : Whether or not said workspace exists 
	*/
	bool has_workspace(std::string const& name);

	/*
		@function currentWorkspaceState

		@return : The current workspace state
	*/
	std::shared_ptr<workspaceState> currentWorkspaceState() { return mCurrentWorkspaceState; };

	/*
		@function target

		@return : The part that is worked on in the document, as a part
	*/
	std::shared_ptr<part> target() { return mPart; }

	/*
		@function selectionBuffer

		@return : The selection buffer
	*/
	std::shared_ptr<frameBuffer> selectionBuffer() { return mSelectionBuffer; }

	/*
		@function push_action adds an action to the action stack and executes it

		@param to_push : The action to push
	*/
	void push_action(std::shared_ptr<action> to_push);
	/*
		@function advance_action_index moves in toward the present in the actions timeline
		if it current position is in the past, it executes all actions it passes

		@param amount [defaults to 1] : The number of step to take toward the present
	*/
	void advance_action_index(unsigned int amount = 1);
	/*
		@function rewind_action_index moves in toward the past in the actions timeline
		it executes all the inverse of the actions it passes

		@param amount [defaults to 1] : The number of step to take toward the past
	*/
	void rewind_action_index(unsigned int amount = 1);

	/*
		@function selection_size 

		@return : The size of the selection buffer
	*/
	unsigned int selection_size() const { return mSelection.size(); }
	/*
		@function selection_at gives a selection out of the selection buffer at a linear index

		@param ind : The index of the wanted selection

		@return : The selection at the required index or an empty selection
	*/
	selection selection_at(unsigned int ind);
	/*
		@function selection_ind gives the linear index of the selection buffer at a ivec3 index

		@param id : The ivec3 id of the wanted selection

		@return : The linear index of the required selection or -1
	*/
	int selection_ind(glm::ivec3 id);
	/*
		@function clear_selection clears the selection buffer
	*/
	void clear_selection();
	/*
		@function toggle_select manages the selection buffer with a toggle of an entity@

		@param id : 		The selection id of the entity to toggle
		@param camState : 	The state of the camera at the moment of calling
		@param additive : 	If the toggle behavior is additive (e.g. ctrl + select)
	*/
	void toggle_select(glm::ivec3 id, camState cam, bool additive);

	void toggle_select(std::shared_ptr<entity> ent, bool additive);
};

#endif