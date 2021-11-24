
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

#ifndef SIMPLESELECTOR_TOOL_HPP_
#define SIMPLESELECTOR_TOOL_HPP_

#include <Tools/Tool.hpp>
#include <Drawables/Base/Drawable.hpp>
#include <Workspaces/Document.hpp>

/*
	@class SimpleSelector_tool describes a tool used to select entities in a Document
*/
template<typename wst>
class SimpleSelector_tool : public tool<wst> {
protected:
	Drawable* mCurrentHover; // The entity under the mouse, if there is one
	int mFilter;
public: 
	/*
		@function SimpleSelector_tool creates a SimpleSelector_tool object 

		@param env : The workspace owning the tool
	*/
	SimpleSelector_tool(wst* env): 
		tool<wst>(env),
		mCurrentHover(nullptr), // No entity is under the mouse
		mFilter(DRAWABLE)
	{
		
	}
	virtual ~SimpleSelector_tool() {};

	virtual bool should_hover(Drawable* drw) {	return drw->type() & mFilter; }

	/*
		@function manage_button_press manages selection when clicking at a point on screen

		@param event : The event handed out by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event)
	{
		if(tool<wst>::mEnv->state() && event->button == 1) {
			// Here it lets the Document manage it's selection
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
		return tool<wst>::mEnv->state()->doc->target()->closest(pt, tool<wst>::mEnv->state()->cam, mFilter).ent;
	}
};

#endif