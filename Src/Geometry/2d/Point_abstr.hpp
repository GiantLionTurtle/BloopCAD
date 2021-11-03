
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

#ifndef POINT_ABSTR2_HPP_
#define POINT_ABSTR2_HPP_

#include "Geom2d_abstr.hpp"

#include <Utils/Param.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Geom2d {

bool point_within(glm::vec2 pt, glm::vec2 top_left, glm::vec2 bottom_right);

template <class Impl>
class Point_abstr : public Geom2d_abstr<Point_abstr<Impl>> {
public:
	Point_abstr()
		: Geom2d_abstr<Point_abstr<Impl>>(POINT)
	{

	}
	glm::vec2 pos()
	{
		return static_cast<Impl*>(this)->pos_impl();
	}

	glm::vec2 at_geom(float t) { return pos(); }
	bool within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		return point_within(pos(), top_left, bottom_right);
	}
	float closest_to_point_interp_geom(glm::vec2 const& pt) { return 0.0f; }
	glm::vec2 closest_to_point_geom(glm::vec2 const& pt) { return pos(); }
	float dist_to_point_geom(glm::vec2 const& pt)
	{
		return glm::distance(pos(), pt);
	}
	float dist_to_point_2(glm::vec2 const& pt)
	{
		return glm::distance2(pos(), pt);
	}
};

class Point : public Point_abstr<Point> {
private:
	Param mX, mY;
public:
	Point(glm::vec2 pt)
		: mX(pt.x)
		, mY(pt.y)
	{

	}
	Point()
		: mX(0.0)
		, mY(0.0)
	{

	}

	glm::vec2 pos_impl()
	{
		return glm::vec2(mX.val(), mY.val());
	}

	Param* x() { return &mX; }
	Param* y() { return &mY; }

	double* x_raw() { return &mX.val_raw(); }
	double* y_raw() { return &mY.val_raw(); }
};

// #include "Point_abstr.cpp"

} // !Geom2d

#endif