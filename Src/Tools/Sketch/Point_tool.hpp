
#ifndef POINT_TOOL_HPP_
#define POINT_TOOL_HPP_

#include <Tools/Tool.hpp>

#include <Drawables/Sk/SkPoint.hpp>
#include <Workspaces/Sketch_ws.hpp>

class Point_tool : public tool<Sketch_ws> {
private:
	SkPoint* mCurrentPoint;
public:
	/*
		@function Line_tool creates a Line_tool object

		@param env : The workspace that owns the tool
	*/
	Point_tool(Sketch_ws* env);

	virtual void init();

	/*
		@function manage_button_press starts or end&push lines on sketch

		@param event : The button press event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "point"; }
};

#endif