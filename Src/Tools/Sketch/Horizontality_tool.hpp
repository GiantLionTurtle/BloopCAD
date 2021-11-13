
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

#ifndef HORIZONTALITY_TOOL_HPP_
#define HORIZONTALITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Horizontality_tool : public Constraint_tool {
public:
	Horizontality_tool(Sketch_ws* env);

	std::string name() { return "horizontality"; }
// protected:
	int could_add_geom(SkGeometry* geom);

	void create_constraint(Constraint_abstr*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct);
};

#endif