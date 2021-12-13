
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
#include "Constraint_tool.hpp"

class Dimension_tool : public Tool<Sketch_ws> {
private:
	int mMode;
public:
	Dimension_tool(Sketch_ws* env);

	virtual void init();

	virtual int could_add_geom(SkGeometry* geom);

	virtual void create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct);

	std::string name() { return "dimension"; }
};

#endif