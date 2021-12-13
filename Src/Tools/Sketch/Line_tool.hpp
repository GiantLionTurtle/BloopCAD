
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

#ifndef LINE_TOOL_HPP_
#define LINE_TOOL_HPP_

#include <Tools/Tool.hpp>

#include <Drawables/Sk/SkPoint.hpp>
#include <Drawables/Sk/SkLine.hpp>
#include <Workspaces/Sketch_ws.hpp>

#include <glm/glm.hpp>

/*
	@class Line_tool describes the tool used to make lines and polylines on a sketch
	@parent : Tool_abstract
*/
class Line_tool : public Tool<Sketch_ws> {
private:
	SkPoint* mEndPos; // end pos of the last line added
	SkLine* mLinePreview, *mLastAdded;
	bool mStarted; // Whether or not the drawing of the line has started
public:
	/*
		@function Line_tool creates a Line_tool object

		@param env : The workspace that owns the tool
	*/
	Line_tool(Sketch_ws* env);

	/*
		@function init makes sure the started flag is down
	*/
	void init();
	void finish();
	
	virtual bool manage_key_press(GdkEventKey* event);

	/*
		@function manage_mouse_move will update a temporary line to display a non placed line

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_button_press starts or end&push lines on sketch

		@param event : The button press event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "line"; }

	SkPoint* add_point(glm::vec2 pt);

	SkLine* lastAdded() { return mLastAdded; }
};

#endif