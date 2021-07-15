
#ifndef WORKSPACE_HPP_
#define WORKSPACE_HPP_

#include <forward_bloop.hpp>
#include <graphics_utils/Camera.hpp>
#include <tools/tool.hpp>
#include <graphics_utils/FrameBuffer.hpp>
#include <Drawables/Drawable.hpp>
#include <tools/tools_forward.hpp>

#include <gtkmm.h>

#include <map>
#include <string>
#include <memory>

struct workspaceState;
using workspaceState_ptr = std::shared_ptr<workspaceState>;

/*
	@struct workspaceState describes the state of a given workspace within a specific document
	the goal is to not duplicate every workspace for every document but rather have each document record
	a state in which each workspace is in this document
*/
struct workspaceState {
	document* doc; // The document owning the state
	Drawable* target; // The target entity of the workspace
	Camera_ptr cam; // The Camera rendering the scene for that workspace in the doc
	CameraState startCamState;
	tool_abstract_ptr currentTool; // The tool being used by the workspace in the doc
	int workspaceName; // The name of the workspace it is describing
};

class workspace;
using workspace_ptr = std::shared_ptr<workspace>;

class workspace {
protected:
	std::shared_ptr<orbit_tool> mOrbit_tool;

	std::map<std::string, tool_abstract_ptr> mTools; // All the tools used by the workspace
	tool_abstract_ptr mDefaultTool; // The tool used by default in the workspace (typically some sort of selector)

	workspaceState_ptr mState; // The state used by the current document

	Gtk::Box* mUpperBar; // The upper bar, where all buttons live
	bloop* mParentBloop; // The parent window
public:
	/*
		@function workspace creates an empty workspace

		@param parent : The parent window of the workspace
	*/
	workspace(bloop* parent);
	/*
		@function workspace creates a workspace with an upper bar and navigation tools

		@param upperBarID : The name of the upperbar in the xml design file
		@param builder : 	The builder provided by gtk that has loaded the design file
		@param : 			The parent window of the workspace
	*/
	workspace(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	// bool invoke_from_key(unsigned int key, std::string& toolName);

	/*
		@function set_tool sets the current tool to a named tool if it exists

		@param name : The name of the tool to set

		@return : Wheter or not the tool exists
	*/
	virtual bool set_tool(int name);
	/*
		@function set_tool sets the current tool to a given tool

		@param tool_ : The tool to set as active
	*/
	void set_tool(tool_abstract_ptr tool_);
	/*
		@function set_toolCursor sets a tool's cursor to it's 
		own cursor

		@param tool_ : The tool from which to extract the cursor info
	*/
	void set_toolCursor(tool_abstract_ptr tool_);

	std::shared_ptr<orbit_tool> orbit() { return mOrbit_tool; }
	/*
		@function defaultTool

		@return : The workspace's default tool
	*/
	tool_abstract_ptr defaultTool() { return mDefaultTool; }

	/*
		@function manage_key_press passes the key presses to the current tool and may
		switch to the default tool if esc is pressed
	*/
	virtual bool manage_key_press(GdkEventKey* event);
	/*
		@function manage_key_release passes the key releases to the current tool
	*/
	virtual bool manage_key_release(GdkEventKey* event);
	/*
		@function manage_mouse_move passes the motion events to the current tool
	*/	
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_mouse_scroll passes the scroll events to the current tool
	*/
	virtual bool manage_mouse_scroll(GdkEventScroll* event) { return true; }
	/*
		@function manage_button_press passes the button presses to the current tool
		and activates the orbit tool if the middle mouse is pressed
	*/		
	virtual bool manage_button_press(GdkEventButton* event);
	/*
		@function manage_button_release passes the button releases to the current tool
	*/
	virtual bool manage_button_release(GdkEventButton* event);

	/*
		@function state 

		@return : The current assigned state from the active document
	*/
	workspaceState_ptr state() { return mState; }
	/*
		@function set_state sets the current state from the document

		@param state_ : The new state of the workspace from the document
	*/
	void set_state(workspaceState_ptr state_);

	/*
		@function upperBar

		@return : The workspace's whole upper bar
	*/
	Gtk::Box* upperBar() { return mUpperBar; }

	virtual std::string name() { return "no workspace"; }
};

#endif