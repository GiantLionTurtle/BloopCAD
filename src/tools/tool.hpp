
#ifndef TOOL_HPP_
#define TOOL_HPP_

#include <forward_bloop.hpp>
#include <entities/entity.hpp>

#include <glm/glm.hpp>
#include <gtkmm.h>

#include <memory>

class workspace; // Forward declaration necesary for pointer

struct compositeCursor;
using compositeCursor_ptr = std::shared_ptr<compositeCursor>;

class tool_abstract;
using tool_abstract_ptr = std::shared_ptr<tool_abstract>;

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
	compositeCursor_ptr mCursor; // The tool's cursor
public:
	/*
		@function tool_abstract creates a tool_abstract object with the default cursor

		@param env : The workspace owning the tool
	*/
	tool_abstract(): mCursor(compositeCursor_ptr(new compositeCursor{Gdk::Cursor::create(Gdk::ARROW), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})) {};
	/*
		@function tool_abstract creates a tool_abstract object with a given cursor

		@param env : 		The workspace owning the tool
		@param cursor_ : 	The cursor the tool will have	
	*/
	tool_abstract(compositeCursor_ptr cursor_): mCursor(cursor_) {};

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
	compositeCursor_ptr cursor() const { return mCursor; };

	virtual void act_on_entity(entity_ptr ent) {};
	virtual bool should_hover(entity_ptr ent) { return true; }

	virtual std::string name() { return "unkown tool"; }
};

template<typename wst>
class tool : public tool_abstract {
protected:
	static_assert(std::is_base_of<workspace, wst>::value, "Workspace type must derive from workspace");
    wst* mEnv;
public:
    tool(wst* env): mEnv(env) {}
    tool(wst* env, compositeCursor_ptr cursor_): tool_abstract(cursor_), mEnv(env) {}
};

#endif