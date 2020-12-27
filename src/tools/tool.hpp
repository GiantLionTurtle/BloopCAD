
#ifndef TOOL_HPP_
#define TOOL_HPP_

#include <forward_bloop.hpp>

#include <glm/glm.hpp>
#include <gtkmm.h>

#include <memory>

class workspace; // Forward declaration necesary for pointer
class entity;

/*
	@struct compositeCursor describes a cursor that has both an image and a cursor type
*/
struct compositeCursor {
	Glib::RefPtr<Gdk::Cursor> windowCursor; // Internal gtk cursor
	Gtk::Image* additionalImage; // Cursor icon
	glm::vec2 upRight, downLeft; // Unused for now
};

/*
	@class tool_abstract describes a wire frame for tools
*/
class tool_abstract {
protected:
	workspace* mEnv; // The workspace owning the tool
	std::shared_ptr<compositeCursor> mCursor; // The tool's cursor
public:
	/*
		@function tool_abstract creates a tool_abstract object with the default cursor

		@param env : The workspace owning the tool
	*/
	tool_abstract(workspace* env): mEnv(env), mCursor(std::shared_ptr<compositeCursor>(new compositeCursor{Gdk::Cursor::create(Gdk::ARROW), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})) {};
	/*
		@function tool_abstract creates a tool_abstract object with a given cursor

		@param env : 		The workspace owning the tool
		@param cursor_ : 	The cursor the tool will have	
	*/
	tool_abstract(workspace* env, std::shared_ptr<compositeCursor> cursor_): mEnv(env), mCursor(cursor_) {};

	/*
		@function init is an overridable init routine
	*/
	virtual void init() {};
	/*
		@function finish is an overridable finish routine
	*/
	virtual void finish() {};

	/*
		@function manage_key_press is an overridable routine called when a key is pressed

		@param event : The key event handed by gtk
	*/
	virtual bool manage_key_press(GdkEventKey* event) { return true; };
	/*
		@function manage_key_release is an overridable routine called when a key is released

		@param event : The key event handed by gtk
	*/	
	virtual bool manage_key_release(GdkEventKey* event) { return true; };
	/*
		@function manage_mouse_move is an overridable routine called when the pointer moves

		@param event : The motion event handed by gtk
	*/	
	virtual bool manage_mouse_move(GdkEventMotion* event) { return true; }
	/*
		@function manage_scroll is an overridable routine called when a scroll is emitted

		@param event : The scroll event handed by gtk
	*/	
	virtual bool manage_scroll(GdkEventScroll* event) { return true; }
	/*
		@function manage_button_press is an overridable routine called when a button is pressed

		@param event : The button event handed by gtk
	*/	
	virtual bool manage_button_press(GdkEventButton* event) { return true; }
	/*
		@function manage_button_release is an overridable routine called when a button is released

		@param event : The button event handed by gtk
	*/	
	virtual bool manage_button_release(GdkEventButton* event) { return true; }

	/*
		@function cursor 

		@return : The cursor of the tool
	*/
	std::shared_ptr<compositeCursor> cursor() const { return mCursor; };

	virtual void notify_selectedEntity(std::shared_ptr<entity> ent) {};
	virtual bool should_hover(std::shared_ptr<entity> ent) { return true; }
};

#endif