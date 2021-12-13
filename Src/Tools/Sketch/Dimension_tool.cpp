
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

#include "Dimension_tool.hpp"

Dimension_tool::Dimension_tool(Sketch_ws* env)
	: Tool<Sketch_ws>(env)
{

}

void Dimension_tool::init()
{
	// Constraint_tool::init();
	std::cout<<"Start dimension tool!\n";
}

int Dimension_tool::could_add_geom(SkGeometry* geom)
{
	return 0;
}

void Dimension_tool::create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct) 
{

}
