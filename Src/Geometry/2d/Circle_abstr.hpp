
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

#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "Point_abstr.hpp"
#include "Line_abstr.hpp"

#include <memory>

namespace Geom2d {

template<typename Impl>
class Circle_abstr : public Geom2d_abstr<Circle_abstr<Impl>> {
public:
	Circle_abstr();
	glm::vec2 center_pos();
	float radius_val();

	glm::vec2 at_geom(float t);
	bool within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp_geom(glm::vec2 const& pt);
	glm::vec2 closest_to_point_geom(glm::vec2 const& pt);
	float dist_to_point_geom(glm::vec2 const& pt);

	bool intersects(Simple_line& l);
};

class Circle : public Circle_abstr<Circle> {
private:
	Geom2d::Point mCenter;
	ExpVar mRadius;
	double* mParams[3];
public:
	Circle(glm::vec2 c, float r);
	Circle();

	glm::vec2 center_pos_impl() { return mCenter.pos(); }
	float radius_val_impl() { return mRadius.eval(); }

	Geom2d::Point* center() { return &mCenter; }
	ExpVar* radius() { return &mRadius; }

	int n_params() { return 3; }
	double** params() { return &mParams[0]; }
private:
	void init_params();
};

#include "Circle_abstr.cpp"

} // !Geom2d

#endif