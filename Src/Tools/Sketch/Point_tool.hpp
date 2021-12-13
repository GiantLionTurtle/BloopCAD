
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POINT_TOOL_HPP_
#define POINT_TOOL_HPP_

#include <Tools/Tool.hpp>

#include <Drawables/Sk/SkPoint.hpp>
#include <Workspaces/Sketch_ws.hpp>

class Point_tool : public Tool<Sketch_ws> {
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