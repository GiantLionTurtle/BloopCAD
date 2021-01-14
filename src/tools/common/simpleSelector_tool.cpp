
#include "simpleSelector_tool.hpp"

#include <graphics_utils/GLCall.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

simpleSelector_tool::simpleSelector_tool(workspace* env): 
	tool_abstract(env),
	mCurrentHover(nullptr), // No entity is under the mouse
	mFilter([](entity_ptr) { return true; })
{
	
}

bool simpleSelector_tool::should_hover(entity_ptr ent)
{
    return mFilter(ent);
}

bool simpleSelector_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		// Here it lets the document manage it's selection
		// it hands over the shift&ctrl
		// mEnv->state()->doc->toggle_select(
		// 	id_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y)), 
		// 	mEnv->state()->cam->state(),
		// 	event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK);		
		entity_ptr ent = entity_at_point(glm::vec2(event->x, event->y));
		if(ent) {
			std::cout<<"Clicked ont entity: "<<ent->name()<<"\n";
		} else {
			std::cout<<"No clicked entity :(\n";
		}
	}
	return true;
}
bool simpleSelector_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state()) {
		entity_ptr ent = entity_at_point(glm::vec2(event->x, event->y));
		// Toggle hovering
		if(ent != mCurrentHover) {
			if(mCurrentHover) {
				mCurrentHover->set_hover(false);
			}
			if(ent) {
				ent->set_hover(true);
			}
			mCurrentHover = ent;
		}
	}
	return true;
}

entity_ptr simpleSelector_tool::entity_at_point(glm::vec2 point)
{
	return mEnv->state()->doc->target()->hovered_child(mEnv->state()->cam, point, mFilter);
}