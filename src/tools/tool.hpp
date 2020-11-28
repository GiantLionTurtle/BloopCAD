
#ifndef TOOL_HPP_
#define TOOL_HPP_

#include <forward_bloop.hpp>
#include <entities/part.hpp>

#include <memory>

#include <glm/glm.hpp>
#include <gtkmm.h>

class workspace;

struct compositeCursor {
	Glib::RefPtr<Gdk::Cursor> windowCursor;
	Gtk::Image* additionalImage;
	glm::vec2 upRight, downLeft;
};

class tool_abstract {
protected:
	workspace* mEnv;
	std::shared_ptr<compositeCursor> mCursor;
public:
	tool_abstract(workspace* env): mEnv(env), mCursor(std::shared_ptr<compositeCursor>(new compositeCursor{Gdk::Cursor::create(Gdk::ARROW), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})) {};
	tool_abstract(workspace* env, std::shared_ptr<compositeCursor> cursor_): mEnv(env), mCursor(cursor_) {};

	virtual void finish() {};
	virtual void init() {};

	std::shared_ptr<compositeCursor> cursor() { return mCursor; };

	virtual bool manage_key_press(GdkEventKey* event) { return true; };
	virtual bool manage_key_release(GdkEventKey* event) { return true; };
	virtual bool manage_mouse_move(GdkEventMotion* event) { return true; }
	virtual bool manage_scroll(GdkEventScroll* event) { return true; }
	virtual bool manage_button_press(GdkEventButton* event) { return true; }
	virtual bool manage_button_release(GdkEventButton* event) { return true; }
};

template<typename actOn>
class tool : public tool_abstract {
protected:
	std::shared_ptr<actOn> mTarget;

public:
    tool(workspace* env): tool_abstract(env) {};

	void set_target(std::shared_ptr<actOn> target) 
	{
		mTarget = target;
	}
};

class line_tool : public tool<sketch> {
public:
	line_tool(workspace* env): tool(env) {};
};

class extrusion_tool : public tool<part> {
public:
	extrusion_tool(workspace* env): tool(env) {};
};

#endif