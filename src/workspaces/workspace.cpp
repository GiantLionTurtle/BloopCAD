
#include "workspace.hpp"
#include <utils/DebugUtils.hpp>
#include <bloop.hpp>
#include <tools/navigation/navigation.hpp>

workspace::workspace(bloop* parent) :
	mParentBloop(parent),
	mDefaultTool(nullptr)
{

}

workspace::workspace(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	mParentBloop(parent),
	mState(new workspaceState),
	mDefaultTool(nullptr)
{
	// Create the upper bar with the builder
	builder->get_widget(upperBarID, mUpperBar);

	// Create navigation/selection tools that are fairly common amongst workspaces
	mOrbit_tool	= std::make_shared<orbit_tool>(this);
}

bool workspace::manage_key_press(GdkEventKey* event)
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
bool workspace::manage_key_release(GdkEventKey* event)
{
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_key_release(event); // Just pass the event deeper
	}
	return true;
}
bool workspace::manage_mouse_move(GdkEventMotion* event)
{
	if(event->state & GDK_BUTTON2_MASK) { // Override the current tool and update the orbit
		set_toolCursor(mOrbit_tool);
		return mOrbit_tool->manage_mouse_move(event);
	}

	if(mState && mState->currentTool) {
		return mState->currentTool->manage_mouse_move(event); // Just pass the event deeper
	}
	return true;
}

bool workspace::manage_button_press(GdkEventButton* event)
{
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_press(event); // Just pass the event deeper
	}
	return true;
}
bool workspace::manage_button_release(GdkEventButton* event)
{
	if(event->state & GDK_BUTTON2_MASK) { // Clean up the orbit override
		mOrbit_tool->finish();
		set_toolCursor(mState->currentTool);
	}
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_release(event); // Just pass the event deeper
	}
	return true;
}

bool workspace::set_tool(int name) 
{
	tool_abstract_ptr to_set = nullptr;
	switch(name) {
	case TOOLIDS::TOOLID_ORBIT:
		to_set = mOrbit_tool;
		break;
	default:
		return false;
	}
	set_tool(to_set);
	return true;
}

void workspace::set_tool(tool_abstract_ptr tool_)
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
void workspace::set_toolCursor(tool_abstract_ptr tool_)
{
	mParentBloop->set_cursor(tool_->cursor());
}

void workspace::set_state(workspaceState_ptr state_) 
{ 
	if(mState) {
		mState = state_;
	} else {
		LOG_WARNING("No state available.");
	}
}