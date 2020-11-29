
#include "workspace.hpp"
#include <utils/errorLogger.hpp>
#include <bloop.hpp>
#include <tools/navigation/navigation.hpp>
#include <tools/common_tools/simpleSelector_tool.hpp>

int workspace::count = 0;

workspace::workspace(bloop* parent) :
	mParentBloop(parent),
	mDefaultTool(nullptr)
{

}

workspace::workspace(std::string const& upperBarID, Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	mParentBloop(parent),
	id(++count),
	mState(new workspaceState),
	mDefaultTool(nullptr)
{
	builder->get_widget(upperBarID, mUpperBar);
	
	mTools["orbit"] 				= std::shared_ptr<tool_abstract>(new orbit_tool(this));
	mTools["zoom"] 					= std::shared_ptr<tool_abstract>(new zoom_tool(this));
	mTools["pan"] 					= std::shared_ptr<tool_abstract>(new pan_tool(this));
	mTools["simpleSelector"] 		= std::shared_ptr<tool_abstract>(new simpleSelector_tool(this));
}

bool workspace::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_Escape) {
		if(mState && mState->currentTool) {
			mState->currentTool->finish();
			set_tool(mDefaultTool);
		}
	} else if(mState && mState->currentTool) {
		return mState->currentTool->manage_key_press(event);
	}
	return true;
}
bool workspace::manage_key_release(GdkEventKey* event)
{
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_key_release(event);
	}
	return true;
}
bool workspace::manage_mouse_move(GdkEventMotion* event)
{
	if(event->state & GDK_BUTTON2_MASK) {
		set_tool_cursor(mTools.at("orbit"));
		return mTools.at("orbit")->manage_mouse_move(event);
	}

	if(mState && mState->currentTool) {
		return mState->currentTool->manage_mouse_move(event);
	}
	return true;
}
bool workspace::manage_mouse_scroll(GdkEventScroll* event)
{
	return mTools.at("zoom")->manage_scroll(event);
}

bool workspace::manage_button_press(GdkEventButton* event)
{
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_press(event);
	}
	return true;
}
bool workspace::manage_button_release(GdkEventButton* event)
{
	if(event->state & GDK_BUTTON2_MASK) {
		mTools.at("orbit")->finish();
		set_tool_cursor(mState->currentTool);
	}
	if(mState && mState->currentTool) {
		return mState->currentTool->manage_button_release(event);
	}
	return true;
}

bool workspace::invoke_from_key(unsigned int key, std::string& toolName)
{
	if(mKey_invokes.find(key) != mKey_invokes.end()) {
		toolName = mKey_invokes[key];
		return true;
	}
	return false;
}

bool workspace::set_tool(std::string const& name) 
{	
	if(mTools.find(name) != mTools.end()) {
		set_tool(mTools.at(name));
		return true;
	} else {
		LOG_WARNING("Trying to set \"" + name + "\" as current tool. There is no such tool.");
	}
	return false;
}
void workspace::set_tool(std::shared_ptr<tool_abstract> tool_)
{
	if(!tool_) {
		LOG_WARNING("Attempting to set null tool.");
		return;
	}
	if(mState->currentTool)
		mState->currentTool->finish();
	mState->currentTool = tool_;
	mState->currentTool->init();
	set_tool_cursor(mState->currentTool);
}
void workspace::set_tool_cursor(std::shared_ptr<tool_abstract> tool_)
{
	mParentBloop->set_cursor(tool_->cursor());
}

void workspace::set_state(std::shared_ptr<workspaceState> state_) 
{ 
	if(mState) {
		mState = state_;
	} else {
		LOG_WARNING("No state available.");
	}
}