
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
#include <entities/entityView.hpp>

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
	entity_ptr ent; // The selected entity
	cameraState camSt; // The camera state at the moment of the selection

	/*
		@function selection creates a selection object

		@param ent_ : 	The selected entity
		@param camSt_ : The camera state during selection
	*/
	selection(entity_ptr ent_, cameraState camSt_) :
		ent(ent_),
		camSt(camSt_)
	{}
};

class document;
using document_ptr = std::shared_ptr<document>;

/*
	@class document represents a document that the user is working with
	@parent Gtk::Box
*/
class document : public Gtk::Box {
private:
	std::shared_ptr<part> mPart; // Curently, documents only work on a single part
	std::vector<selection> mSelection; // This will be kept linear for as long as there is no need for large selections

	std::map<std::string, workspaceState_ptr> mWorkspaceStates; // All the workspace states for the various workspaces
	workspaceState_ptr mCurrentWorkspaceState; // The workspace state for the current workspace working on the document

	std::vector<std::shared_ptr<action>> mActionStack; // The stack of actions done in the document
	int mActionInd, mCurrentActionNum, mActionStackSize; // Metrics for the action stack

	glm::vec3 mBackgroundColor; // The rendering background color
	Gtk::GLArea mViewport; // The rendering widget
	int mFrameId; // The current frame id, it is NOT garanteed to be unique
	bloop* mParentBloop; // The bloop window owning the document
	cameraState mCurrentCamState;

	bool mRequire_redraw;
	bool mUseSelectionBuffer;
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
	void do_resize(int width, int height);
	/*
		@function do_render is the main rendering loop, entities draw themselves
		and their selection shadows in this function
	*/
	bool do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */);
	/*
		@function frame_callback calls the rendering loop on at a fixed interval
	*/
	static gboolean frame_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer data);
	
	bool manage_key_press(GdkEventKey* event);
	bool manage_key_release(GdkEventKey* event);

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
	workspace_ptr set_workspace(std::string const& name);
	/*
		@function set_workspace enforces the current workspace in the bloop window

		@return : The set workspace, or nullptr 
	*/
	workspace_ptr set_workspace();
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
	workspaceState_ptr state() { return mCurrentWorkspaceState; };

	/*
		@function target

		@return : The part that is worked on in the document, as a part
	*/
	std::shared_ptr<part> target() { return mPart; }

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

	void update_actionStack();

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
		@function selection_ind gives the linear index of the selection buffer for a peculiar entity

		@param id : The entity that is looked for

		@return : The linear index of the required selection or -1
	*/
	int selection_ind(entity_ptr ent);
	/*
		@function clear_selection clears the selection buffer
	*/
	void clear_selection();

	void toggle_select(entity_ptr ent, cameraState cam, bool additive);
	void toggle_select(entity_ptr ent, bool additive);
};

#endif