
// BloopCAD
// Copyright (C) 2020  BloopCorp

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