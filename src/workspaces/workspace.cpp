
#include "workspace.hpp"
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
			mState->currentTool = mDefaultTool;
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
	if(event->state * GDK_BUTTON2_MASK) {
		mTools.at("orbit")->finish();
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
		if(mState->currentTool)
			mState->currentTool->finish();
		mState->currentTool = mTools[name];
		mState->currentTool->init();
		return true;
	} else {
		LOG_WARNING("Trying to set \"" + name + "\" as current tool. There is no such tool.");
	}
	return false;
}

void workspace::set_state(std::shared_ptr<workspaceState> state_) 
{ 
	if(mState) {
		mState = state_;
	} else {
		LOG_WARNING("No state available.");
	}
}

	// std::cout<<"-----------------------\n";
	// std::cout<<event->state<<"\n";
	// std::cout<<(event->state & GDK_SHIFT_MASK) << "\n";
	// std::cout<<(event->state & GDK_LOCK_MASK) << "\n";
	// std::cout<<(event->state & GDK_CONTROL_MASK) << "\n";
	// std::cout<<(event->state & GDK_MOD1_MASK) << "\n";
	// std::cout<<(event->state & GDK_MOD2_MASK) << "\n";
	// std::cout<<(event->state & GDK_MOD3_MASK) << "\n";
	// std::cout<<(event->state & GDK_MOD4_MASK) << "\n";
	// std::cout<<(event->state & GDK_MOD5_MASK) << "\n";
	// std::cout<<(event->state & GDK_BUTTON1_MASK) << "\n";
	// std::cout<<(event->state & GDK_BUTTON2_MASK) << "\n";
	// std::cout<<(event->state & GDK_BUTTON3_MASK) << "\n";
	// std::cout<<(event->state & GDK_BUTTON4_MASK) << "\n";
	// std::cout<<(event->state & GDK_BUTTON5_MASK) << "\n";
	// std::cout<<"-----------------------\n";