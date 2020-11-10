
#ifndef TOOL_HPP_
#define TOOL_HPP_

#include <forward_bloop.hpp>
// #include "document.hpp"
#include <entities/part.hpp>

#include <memory>

#include <glm/glm.hpp>

#include <gtkmm.h>

class workspaceState;

class tool_abstract {
protected:
	std::shared_ptr<std::shared_ptr<workspaceState>> mWorkspaceState;
public:
	tool_abstract(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): mWorkspaceState(workspaceState_) {}
	virtual void finish() {};
	virtual void init() {};

	virtual bool manage_key_press(GdkEventKey* event){ return true; };
	virtual bool manage_mouse_move(GdkEventMotion* event) { return true; }
	virtual bool manage_scroll(GdkEventScroll* event) { return true; }
	virtual bool manage_button_press(GdkEventButton* event) { return true; }
	virtual bool manage_button_release(GdkEventButton* event) { return true; }

	void set_state(std::shared_ptr<workspaceState> state) { (*mWorkspaceState) = state; }
};

template<typename actOn>
class tool : public tool_abstract {
protected:
	std::shared_ptr<actOn> mTarget;

public:
    tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool_abstract(workspaceState_) {};

	void set_target(std::shared_ptr<actOn> target) 
	{
		mTarget = target;
	}
};


class simpleSelector_tool : public tool_abstract {
private:
	std::shared_ptr<entity> mCurrentHover;
public: 
	simpleSelector_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool_abstract(workspaceState_), mCurrentHover(nullptr) {};

	virtual bool manage_mouse_move(GdkEventMotion* event);
};

class line_tool : public tool<sketch> {
public:
	line_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool(workspaceState_) {};
};

class startSketch_tool : public tool<part> {
public:
	startSketch_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool(workspaceState_) {};
};

class extrusion_tool : public tool<part> {
public:
	extrusion_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): tool(workspaceState_) {};
};

#endif