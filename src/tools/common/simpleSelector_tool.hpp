
#ifndef SIMPLESELECTOR_TOOL_HPP_
#define SIMPLESELECTOR_TOOL_HPP_

#include <tools/tool.hpp>
#include <workspaces/workspace.hpp>
#include <Drawables/Drawable.hpp>
#include <document.hpp>

/*
	@class simpleSelector_tool describes a tool used to select entities in a document
*/
template<typename wst>
class simpleSelector_tool : public tool<wst> {
protected:
	Drawable* mCurrentHover; // The entity under the mouse, if there is one
	int mFilter;
public: 
	/*
		@function simpleSelector_tool creates a simpleSelector_tool object 

		@param env : The workspace owning the tool
	*/
	simpleSelector_tool(wst* env): 
		tool<wst>(env),
		mCurrentHover(nullptr), // No entity is under the mouse
		mFilter(DRAWABLE)
	{
		
	}
	virtual ~simpleSelector_tool() {};

	virtual bool should_hover(Drawable* drw) {	return drw->type() & mFilter; }

	/*
		@function manage_button_press manages selection when clicking at a point on screen

		@param event : The event handed out by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event)
	{
		if(tool<wst>::mEnv->state() && event->button == 1) {
			// Here it lets the document manage it's selection
			// it hands over the shift&ctrl
			tool<wst>::mEnv->state()->doc->toggle_select(
				entity_at_point(glm::vec2(event->x, event->y)), 
				tool<wst>::mEnv->state()->cam->state(),
				event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK);		
		}
		return true;
	}
	/*
		@function manage_mouse_move manages hovering when moving the mouse on screen

		@param event : The event handed out by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event)
	{
		if(tool<wst>::mEnv->state()) {
			Drawable* ent = entity_at_point(glm::vec2(event->x, event->y));
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

	virtual std::string name() { return "simple selector"; }

	/*
		@function entity_at_point gives the entity under a certain point on screen, can be overriden to filter out some entities

		@param point : The position on screen, in pixels

		@return : A pointer to the entity under a point if it exists
	*/
	virtual Drawable* entity_at_point(glm::vec2 pt)
	{
		return tool<wst>::mEnv->state()->doc->target()->closest(pt, tool<wst>::mEnv->state()->cam.get(), mFilter).ent;
	}
};

#endif