
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef WORKSPACE_HPP_
#define WORKSPACE_HPP_

#include <Graphics_utils/Camera.hpp>
#include <Tools/Tool.hpp>
#include <Graphics_utils/FrameBuffer.hpp>
#include <Drawables/Base/Drawable.hpp>
#include <Tools/Tools_forward.hpp>
#include <Utils/Expunge.hpp>

#include <gtkmm.h>

#include <map>
#include <string>
#include <memory>

class Bloop;

/*
	@struct WorkspaceState describes the state of a given workspace within a specific Document
	the goal is to not duplicate every workspace for every Document but rather have each Document record
	a state in which each workspace is in this Document
*/
struct WorkspaceState {
	Document* doc; // The Document owning the state
	Camera* cam; // The Camera rendering the scene for that workspace in the doc
	CameraState startCamState;
	Tool_abstract* currentTool; // The tool being used by the workspace in the doc
	int workspaceName; // The name of the workspace it is describing

	WorkspaceState()
		: doc(nullptr)
		, cam(nullptr)
		, currentTool(nullptr)
	{

	}
	~WorkspaceState()
	{
		expunge(cam);
	}
};

class Workspace_abstr {
protected:
	Orbit3d_tool* mOrbit3d_tool;

	std::map<std::string, Tool_abstract*> mTools; // All the tools used by the workspace
	Tool_abstract* mDefaultTool; // The tool used by default in the workspace (typically some sort of selector)

	WorkspaceState* mState; // The state used by the current Document

	Gtk::Box* mUpperBar; // The upper bar, where all buttons live
	Bloop* mParentBloop; // The parent window
public:
	/*
		@function workspace creates an empty workspace

		@param parent : The parent window of the workspace
	*/
	Workspace_abstr(Bloop* parent);
	/*
		@function workspace creates a workspace with an upper bar and navigation tools

		@param upperBarID : The name of the upperbar in the xml design file
		@param builder : 	The builder provided by gtk that has loaded the design file
		@param : 			The parent window of the workspace
	*/
	Workspace_abstr(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent);

	~Workspace_abstr();

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
	void set_tool(Tool_abstract* tool_);
	/*
		@function set_toolCursor sets a tool's cursor to it's 
		own cursor

		@param tool_ : The tool from which to extract the cursor info
	*/
	void set_toolCursor(Tool_abstract* tool_);

	Orbit3d_tool* orbit() { return mOrbit3d_tool; }
	/*
		@function defaultTool

		@return : The workspace's default tool
	*/
	Tool_abstract* defaultTool() { return mDefaultTool; }

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

		@return : The current assigned state from the active Document
	*/
	WorkspaceState* state() { return mState; }
	/*
		@function set_state sets the current state from the Document

		@param state_ : The new state of the workspace from the Document
	*/
	void set_state(WorkspaceState* state_);

	/*
		@function upperBar

		@return : The workspace's whole upper bar
	*/
	Gtk::Box* upperBar() { return mUpperBar; }

	virtual std::string name() { return "no workspace"; }
};

#endif