
#ifndef CIRCLE_TOOL_HPP_
#define CIRCLE_TOOL_HPP_

#include <Tools/Tool.hpp>

#include <Drawables/Sk/SkCircle.hpp>
#include <Workspaces/Sketch_ws.hpp>

#include <glm/glm.hpp>

class Circle_tool : public tool<Sketch_ws> {
private:
	SkCircle* mCirclePreview;
	bool mStarted; // Whether or not the drawing of the circle has started
public:
	Circle_tool(Sketch_ws* env);

	/*
		@function init makes sure the started flag is down
	*/
	void init();
	void finish();

	bool manage_key_press(GdkEventKey* event);

	bool manage_mouse_move(GdkEventMotion* event);
	bool manage_button_press(GdkEventButton* event);

	std::string name() { return "circle"; }
};

#endif