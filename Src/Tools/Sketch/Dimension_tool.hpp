
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

#ifndef DIMENSION_TOOL_HPP_
#define DIMENSION_TOOL_HPP_

#include <Workspaces/Sketch_ws.hpp>
#include <Drawables/Sk/SkGeometry.hpp>
#include "Constraint_tool.hpp"


/*
	Tool for dimensioning part of the sketch, idealy it could do

	* Line length
	* Point to line distance (perpendicular)
	* Point to point distance (vertical or horizontal)
	* Cirle radius
	* Circle diameter
	* Line to circle distance
	* point to circle distance

	* Line line distance (if parallel!) [not for now]

	while transitioning from one to another smoothly

	* Line length => point line distance
	* Line length => circle line distance
	* Line length => line line distance [not for now]

	* Circle radius <=> circle diameter (outside or inside circle)
	* Circle radius => line to circle distance

	* point point distance (horizontal <=> vertical)
*/
class Dimension_tool : public Tool<Sketch_ws> {
private:
	enum Dimension_modes { NONE, LINE_LEN, PTPT_HDIST, PTPT_VDIST, LINEPT_DIST, 
	CIRC_RAD, CIRC_DIAM, CIRCCIRC_DIST, LINECIRC_DIST, PTCIRC_DIST, LINELINE_DIST };

	int mMode { Dimension_modes::NONE };
	SkGeometry* mGeomA { nullptr }, *mGeomB { nullptr };
	SkGeometry* mCurrHov { nullptr };
	int mFilter { Geom2d::ANY };
public:
	Dimension_tool(Sketch_ws* env);

	void init();
	void finish() {}

	bool manage_key_press(GdkEventKey* event) { return true; }
	bool manage_key_release(GdkEventKey* event) { return true; }
	bool manage_mouse_move(GdkEventMotion* event);
	bool manage_scroll(GdkEventScroll* event) { return true; }
	bool manage_button_press(GdkEventButton* event) { return true; }
	bool manage_button_release(GdkEventButton* event);

	void create_constraint();

	void dispatch_mode_none(SkGeometry* geom);
	void dispatch_mode_line_len(SkGeometry* geom);
	void dispatch_mode_circle_dim(SkGeometry* geom);
	void dispatche_mode_pt_dist(SkGeometry* geom);

	std::string name() { return "dimension"; }
};

#endif