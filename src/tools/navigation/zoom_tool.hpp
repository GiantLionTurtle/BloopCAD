
#ifndef ZOOM_TOOL_HPP_
#define ZOOM_TOOL_HPP_

#include <tools/tool.hpp>

class zoom_tool : public tool_abstract {
private:
	glm::vec2 prevPos;
	bool is_moving;

	glm::vec2 zoomStart;
	bool is_zooming;
public:
	zoom_tool(workspace* env);
	
	virtual void finish() { is_moving = false; is_zooming = false; }
	virtual void init() { is_moving = false; }

	virtual bool manage_button_release(GdkEventButton* event) { if(event->state & GDK_BUTTON1_MASK) finish(); return true; }

	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_scroll(GdkEventScroll* event);

	void zoom(glm::vec2 origin, float amount);
};

#endif