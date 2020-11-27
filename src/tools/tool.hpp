
#ifndef TOOL_HPP_
#define TOOL_HPP_

#include <forward_bloop.hpp>
#include <entities/part.hpp>

#include <memory>

#include <glm/glm.hpp>
#include <gtkmm.h>

class workspace;

class tool_abstract {
protected:
	workspace* mEnv;
public:
	tool_abstract(workspace* env): mEnv(env) {}
	virtual void finish() {};
	virtual void init() {};

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