
#ifndef CIRCLE_TOOL_HPP_
#define CIRCLE_TOOL_HPP_

#include <tools/tool.hpp>

#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>
#include <workspaces/sketchDesign.hpp>

#include <glm/glm.hpp>

class circle_tool : public tool<sketchDesign> {
private:
	std::shared_ptr<sketchCircle> mCirclePreview;
	bool mStarted; // Whether or not the drawing of the circle has started
public:
	circle_tool(sketchDesign* env);

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