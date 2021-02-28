
#include "workspace.hpp"
#include <utils/errorLogger.hpp>
#include <bloop.hpp>
#include <tools/navigation/navigation.hpp>
#include <tools/common/simpleSelector_tool.hpp>

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
	mTools["orbit"] 				= tool_abstract_ptr(new orbit_tool(this));
	mTools["zoom"] 					= tool_abstract_ptr(new zoom_tool(this));
	mTools["pan"] 					= tool_abstract_ptr(new pan_tool(this));
	mTools["simpleSelector"] 		= tool_abstract_ptr(new simpleSelector_tool<workspace>(this));
}

bool workspace::manage_key_press(GdkEventKey* event)
{
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
		set_toolCursor(mTools.at("orbit"));
		return mTools.at("orbit")->manage_mouse_move(event);
	}

	if(mState && mState->currentTool) {
		return mState->currentTool->manage_mouse_move(event); // Just pass the event deeper
	}
	return true;
}
bool workspace::manage_mouse_scroll(GdkEventScroll* event)
{
	return mTools.at("zoom")->manage_scroll(event); // The only tool that can be used with scroll is zoom
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
		mTools.at("orbit")->finish();
		set_toolCursor(mState->currentTool);
	}
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_release(event); // Just pass the event deeper
	}
	return true;
}

// bool workspace::invoke_from_key(unsigned int key, std::string& toolName)
// {
// 	if(mKey_invokes.find(key) != mKey_invokes.end()) {
// 		toolName = mKey_invokes[key];
// 		return true;
// 	}
// 	return false;
// }

bool workspace::set_tool(std::string const& name) 
{	
	// This function is a wrapper for the other set_tool that takes a shared pointer
	if(mTools.find(name) != mTools.end()) {
		set_tool(mTools.at(name)); 
		return true;
	} else {
		LOG_WARNING("Trying to set \"" + name + "\" as current tool. There is no such tool.");
	}
	return false;
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