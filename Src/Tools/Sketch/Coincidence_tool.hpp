
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

#ifndef COINCIDENCE_TOOL_HPP_
#define COINCIDENCE_TOOL_HPP_

#include "Constraint_tool.hpp"

class Coincidence_tool : public Constraint_tool {
public:
	Coincidence_tool(Sketch_ws* env);

	std::string name() { return "coincidence"; }
// protected:
	int could_add_geom(SkDrawable* geom);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif