
#ifndef SIMPLESELECTOR_TOOL_HPP_
#define SIMPLESELECTOR_TOOL_HPP_

#include <tools/tool.hpp>
#include <entities/forward_entities.hpp>

#include <functional>
/*
	@class simpleSelector_tool describes a tool used to select entities in a document
*/
class simpleSelector_tool : public tool_abstract {
protected:
	entity_ptr mCurrentHover; // The entity under the mouse, if there is one

	std::function<bool (entity_ptr)> mFilter;
public: 
	/*
		@function simpleSelector_tool creates a simpleSelector_tool object 

		@param env : The workspace owning the tool
	*/
	simpleSelector_tool(workspace* env);
	virtual ~simpleSelector_tool() {};

	virtual bool should_hover(entity_ptr ent);

	/*
		@function manage_button_press manages selection when clicking at a point on screen

		@param event : The event handed out by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event);
	/*
		@function manage_mouse_move manages hovering when moving the mouse on screen

		@param event : The event handed out by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "simple selector"; }

	/*
		@function entity_at_point gives the entity under a certain point on screen, can be overriden to filter out some entities

		@param point : The position on screen, in pixels

		@return : A pointer to the entity under a point if it exists
	*/
	virtual entity_ptr entity_at_point(glm::vec2 point);
};

#endif