
#ifndef POINT_TOOL_HPP_
#define POINT_TOOL_HPP_

#include <Tools/Tool.hpp>

#include <Drawables/sk/SkPoint.hpp>
#include <workspaces/sketchDesign.hpp>

class Point_tool : public tool<sketchDesign> {
private:
	SkPoint* mCurrentPoint;
public:
	/*
		@function Line_tool creates a Line_tool object

		@param env : The workspace that owns the tool
	*/
	Point_tool(sketchDesign* env);

	virtual void init();

	/*
		@function manage_button_press starts or end&push lines on sketch

		@param event : The button press event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "point"; }
};

#endif