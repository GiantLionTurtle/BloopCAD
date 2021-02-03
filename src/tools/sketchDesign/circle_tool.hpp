
#ifndef CIRCLE_TOOL_HPP_
#define CIRCLE_TOOL_HPP_

#include <tools/tool.hpp>

#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>
#include <workspaces/sketchDesign.hpp>

#include <glm/glm.hpp>

class circle_tool : public tool<sketchDesign> {
private:
	std::shared_ptr<sketchCircle> mCircle;
	bool started; // Whether or not the drawing of the circle has started
public:
	circle_tool(sketchDesign* env);

	/*
		@function init makes sure the started flag is down
	*/
	virtual void init();

	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "circle"; }
};

#endif