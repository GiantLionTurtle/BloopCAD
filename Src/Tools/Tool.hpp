
#ifndef TOOL_HPP_
#define TOOL_HPP_

#include <Forward_bloop.hpp>
#include <Drawables/Base/Drawable.hpp>

#include <glm/glm.hpp>
#include <gtkmm.h>

#include <memory>

class Workspace_abstr; // Forward declaration necesary for pointer declaration

/*
	@struct compositeCursor describes a cursor that has both an Image and a cursor type
*/
// struct compositeCursor {
// 	Glib::RefPtr<Gdk::Cursor> windowCursor; // Internal gtk cursor
// 	Gtk::Image* additionalImage; // Cursor icon
// 	glm::vec2 upRight, downLeft; // Unused for now
// };

/*
	@class Tool_abstract describes a wire frame for tools
*/
class Tool_abstract {
protected:
	Glib::RefPtr<Gdk::Cursor> mCursor; // The tool's cursor
public:
	/*
		@function Tool_abstract creates a Tool_abstract object with the default cursor

		@param env : The workspace owning the tool
	*/
	Tool_abstract(): mCursor(Gdk::Cursor::create(Gdk::ARROW)) {}
	/*
		@function Tool_abstract creates a Tool_abstract object with a given cursor

		@param env : 		The workspace owning the tool
		@param cursor_ : 	The cursor the tool will have	
	*/
	Tool_abstract(Glib::RefPtr<Gdk::Cursor> cursor_): mCursor(cursor_) {};

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
	virtual bool manage_key_press(GdkEventKey* event) { return true; }
	/*
		@function manage_key_release is an overridable routine called when a key is released

		@param event : The key event handed by gtk
	*/	
	virtual bool manage_key_release(GdkEventKey* event) { return true; }
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
	Glib::RefPtr<Gdk::Cursor> cursor() const { return mCursor; };

	virtual void act_on_entity(Drawable* ent) {};
	virtual bool should_hover(Drawable* ent) { return true; }

	virtual std::string name() { return "unkown tool"; }

	void load_icon(std::string const& location);
};

template<typename wst>
class tool : public Tool_abstract {
protected:
	static_assert(std::is_base_of<Workspace_abstr, wst>::value, "Workspace type must derive from Workspace_abstr");
    wst* mEnv;
public:
    tool(wst* env): mEnv(env) {}
    tool(wst* env, Glib::RefPtr<Gdk::Cursor> cursor_): Tool_abstract(cursor_), mEnv(env) {}
};

#endif