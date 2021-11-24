
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

#ifndef SKCIRCLE_HPP_
#define SKCIRCLE_HPP_

#include "SkHandleCurve.hpp"
#include "SkCircleCurve.hpp"

class SkCircle : public SkHandleCurve<SkHandleCurve_FixedIndexer<1, SkCircleCurve>> {
public:
	using parentType = SkHandleCurve<SkHandleCurve_FixedIndexer<1, SkCircleCurve>>;
	SkCircle(glm::vec2 center_, float radius_, Geom3d::Plane_abstr* pl, bool fixed_):
		SkHandleCurve({ center_ }, pl, fixed_)
	{
		set_name("SkLine");
		set_radius(radius_);
	}


	SkPoint* center() { return handle(0); }
	glm::vec2 center_pos() { return handle(0)->pos(); }
	void set_center(glm::vec2 pos) { center()->set(pos); }
    var_ptr radius() { return curve()->radius(); }
	float radius_val() { return radius()->eval(); }
	void set_radius(float val) { radius()->drag(val); update(); }

	virtual void release()
	{
		center()->release();
		radius()->set_dragged(false);
	}
	std::vector<var_ptr> all_vars()
	{
		std::vector<var_ptr> vars = parentType::all_vars();
		vars.push_back(radius());
		return vars;
	}

	int selection_rank() { return 3; }
};

#endif