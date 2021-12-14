
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

#ifndef CONSTRAINT_TOOL_HPP_
#define CONSTRAINT_TOOL_HPP_

#include <Workspaces/Sketch_ws.hpp>
#include <Tools/Tool.hpp>

class Constraint_tool : public Tool<Sketch_ws> {
public:
	enum add_states { COULDNT_ADD, COULD_ADD, WOULD_BE_COMPLETE };
protected:
	SkGeometry* mEntA, *mEntB, *mCurrentHover;
	int mFilter;
public:
	Constraint_tool(Sketch_ws* env, int filter = Geom2d::ANY);

	virtual void init();
	virtual void finish();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
// protected:
	virtual int could_add_geom(SkGeometry* geom) = 0;

	virtual void add_geom(SkGeometry* geom);
	void add_constraint();
	void clear_geometries();
	virtual void create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct) = 0;

	// TODO: Inline these thingies?
	static bool is_point(SkGeometry* ent);
	static bool is_line(SkGeometry* ent);
	static bool is_curve(SkGeometry* ent);
	static bool is_point_or_curve(SkGeometry* ent);
};
#endif