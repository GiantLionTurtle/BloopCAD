
#ifndef DOCUMENT_HPP_
#define DOCUMENT_HPP_

#include <forward_bloop.hpp>
#include <workspaces/workspace.hpp>
#include <Tools/Tool.hpp>
#include <graphics_utils/FrameBuffer.hpp>
#include <actions/action.hpp>
#include <Drawables/Part.hpp>
#include <graphics_utils/Camera.hpp>
#include <Drawables/entityView.hpp>
#include "eventsManager.hpp"

#include <gtkmm.h>

#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <memory>

/*
	@struct selection describes a selected entity from a certain Camera viewpoint
*/
struct selection {
	Drawable* ent; // The selected entity
	CameraState camSt; // The Camera state at the moment of the selection

	/*
		@function selection creates a selection object

		@param ent_ : 	The selected entity
		@param camSt_ : The Camera state during selection
	*/
	selection(Drawable* ent_, CameraState camSt_) :
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
	eventsManager* mEventsManager;

	Part* mPart; // Curently, documents only work on a single part
	Drawable* mToolPreview;
	std::vector<selection> mSelection; // This will be kept linear for as long as there is no need for large selections

	// std::map<std::string, workspaceState_ptr> mWorkspaceStates; // All the workspace states for the various workspaces
	workspaceState_ptr mSketchState;
	workspaceState_ptr mPartState;
	workspaceState_ptr mCurrentWorkspaceState; // The workspace state for the current workspace working on the document

	std::vector<std::shared_ptr<action>> mActionStack; // The stack of actions done in the document
	int mActionInd, mCurrentActionNum, mActionStackSize; // Metrics for the action stack

	glm::vec3 mBackgroundColor; // The rendering background color
	Gtk::GLArea mViewport; // The rendering widget
	int mFrameId; // The current frame id, it is NOT garanteed to be unique
	bloop* mParentBloop; // The bloop window owning the document
	CameraState mCurrentCamState;

	bool mNeed_redraw;
	bool mUseSelectionBuffer;
	entityView* mSideBar;

	std::string mName;

	int mVerboseLevel;
public:
	/*
		@function document creates an empty and mostly unitialized document 
	*/
	document(eventsManager* manager = nullptr);
	~document();

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
		@function connect_signals connects the viewport events to bloop's callbacks
	*/
	void connect_signals();

	entityView* sideBar() const { return mSideBar; }

	/*
		@function set_workspace sets the current workspace to a named workspace

		@param name : The name of the wanted workspace

		@return : The set workspace, or nullptr 
	*/
	workspace_ptr set_workspace(int name);
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
	bool has_workspace(int name);

	/*
		@function currentWorkspaceState

		@return : The current workspace state
	*/
	workspaceState_ptr state() { return mCurrentWorkspaceState; };

	bloop* window() { return mParentBloop; }
	void set_parent(bloop* parentBloop);

	std::string name() { return mName; }
	void set_name(std::string const& name_) { mName = name_; }

	/*
		@function target

		@return : The part that is worked on in the document, as a part
	*/
	Part* target() { return mPart; }
	int frameId() { return mFrameId; }
	Drawable* toolPreview() { return mToolPreview; }
	void set_toolPreview(Drawable* prev) { mToolPreview = prev; mNeed_redraw = true; }
	void clear_toolPreview() { set_toolPreview(nullptr); }

	bool need_redraw() { return mNeed_redraw || (mToolPreview && mToolPreview->need_redraw()); }
	bool need_update() { return (mToolPreview && mToolPreview->need_update()); }

	Gtk::GLArea& viewport() { return mViewport; }

	bool update_Camera();

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
	int selection_ind(Drawable* ent);
	/*
		@function clear_selection clears the selection buffer
	*/
	void clear_selection();

	void toggle_select(Drawable* ent, CameraState cam, bool additive);
	void toggle_select(Drawable* ent, bool additive);
};

#endif