
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

#include "Workspace_abstr.hpp"

#include <Utils/Debug_util.hpp>
#include <Bloop/Bloop.hpp>
#include <Tools/Navigation3d/Navigation3d.hpp>
#include "Document.hpp"

Workspace_abstr::Workspace_abstr(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent)
	: mParentBloop(parent)
	, mState(nullptr)
	, mDefaultTool(nullptr)
{
	// Create the upper bar with the builder
	builder->get_widget(upperBarID, mUpperBar);

	// Create navigation/selection tools that are fairly common amongst Workspaces
	mOrbit3d_tool	= new Orbit3d_tool(this);
}
Workspace_abstr::~Workspace_abstr()
{
	expunge(mOrbit3d_tool);
	expunge(mUpperBar);
}

bool Workspace_abstr::manage_key_press(GdkEventKey* event)
{
	if(event->state & GDK_CONTROL_MASK && mState && mState->doc) {
		if(event->keyval == GDK_KEY_z) {
			mState->doc->rewind_action_index();
			return true;
		} else if(event->keyval == GDK_KEY_y) {
			mState->doc->advance_action_index();
			return true;
		}
	}

	if(mState && mState->currentTool) {
		if(mState->currentTool->manage_key_press(event) && event->keyval == GDK_KEY_Escape) {
			mState->currentTool->finish(); // Terminate the tool before switching to the default tool
			set_tool(mDefaultTool);
		}
	}
	return true;
}
bool Workspace_abstr::manage_key_release(GdkEventKey* event)
{
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_key_release(event); // Just pass the event deeper
	}
	return true;
}
bool Workspace_abstr::manage_mouse_move(GdkEventMotion* event)
{
	if(event->state & GDK_BUTTON2_MASK) { // Override the current tool and update the orbit
		set_toolCursor(mOrbit3d_tool);
		return mOrbit3d_tool->manage_mouse_move(event);
	}

	if(mState && mState->currentTool) {
		return mState->currentTool->manage_mouse_move(event); // Just pass the event deeper
	}
	return true;
}

bool Workspace_abstr::manage_button_press(GdkEventButton* event)
{
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_press(event); // Just pass the event deeper
	}
	return true;
}
bool Workspace_abstr::manage_button_release(GdkEventButton* event)
{
	if(event->state & GDK_BUTTON2_MASK) { // Clean up the orbit override
		mOrbit3d_tool->finish();
		set_toolCursor(mState->currentTool);
	}
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_release(event); // Just pass the event deeper
	}
	return true;
}

bool Workspace_abstr::set_tool(int name) 
{
	Tool_abstract* to_set = nullptr;
	switch(name) {
	case TOOLIDS::TOOLID_ORBIT:
		to_set = mOrbit3d_tool;
		break;
	default:
		return false;
	}
	set_tool(to_set);
	return true;
}

void Workspace_abstr::set_tool(Tool_abstract* tool_)
{
	if(!tool_) {
		LOG_WARNING("Attempting to set null tool.");
		return;
	}
	if(mState->currentTool)
		mState->currentTool->finish(); // Terminate the current tool

	// Set and setup the new tool
	mState->currentTool = tool_;
	mState->currentTool->init();
	set_toolCursor(mState->currentTool);
	mParentBloop->notify_set_tool(tool_->name());
}
void Workspace_abstr::set_toolCursor(Tool_abstract* tool_)
{
	mParentBloop->set_cursor(tool_->cursor());
}

void Workspace_abstr::set_state(WorkspaceState* state_) 
{ 
	mState = state_;
}