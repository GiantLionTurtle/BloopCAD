
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

#ifndef POINT_ABSTR2_HPP_
#define POINT_ABSTR2_HPP_

#include "Geom2d_abstr.hpp"

#include <Utils/Param.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Geom2d {

bool point_within(glm::dvec2 pt, glm::dvec2 top_left, glm::dvec2 bottom_right);

template <class Impl>
class Point_abstr : public Geom2d_abstr<Point_abstr<Impl>> {
public:
	Point_abstr()
		: Geom2d_abstr<Point_abstr<Impl>>(POINT)
	{

	}
	glm::dvec2 pos()
	{
		return static_cast<Impl*>(this)->pos_impl();
	}
	bool free_impl()
	{
		return 	static_cast<Impl*>(this)->free_impl2();
	}

	glm::dvec2 at_geom(double t) { return pos(); }
	bool within_geom(glm::dvec2 top_left, glm::dvec2 bottom_right, bool contained)
	{
		return point_within(pos(), top_left, bottom_right);
	}
	double closest_to_point_interp_geom(glm::dvec2 const& pt) { return 0.0f; }
	glm::dvec2 closest_to_point_geom(glm::dvec2 const& pt) { return pos(); }
	double dist_to_point_geom(glm::dvec2 const& pt)
	{
		return glm::distance(pos(), pt);
	}
	double dist_to_point_2(glm::dvec2 const& pt)
	{
		return glm::distance2(pos(), pt);
	}
};

class Point : public Point_abstr<Point> {
private:
	Param mX, mY;
public:
	Point(glm::dvec2 pt)
		: mX(pt.x)
		, mY(pt.y)
	{

	}
	Point()
		: mX(0.0)
		, mY(0.0)
	{

	}

	glm::dvec2 pos_impl()
	{
		return glm::dvec2(mX.val(), mY.val());
	}

	bool free_impl2()
	{
		return 	mX.frozen() == Param::Frozen_levels::UNFROZEN &&
				mY.frozen() == Param::Frozen_levels::UNFROZEN;
	}

	Param* x() { return &mX; }
	Param* y() { return &mY; }

	double x_val() { return mX.val(); }
	double y_val() { return mY.val(); }
};

// #include "Point_abstr.cpp"

} // !Geom2d

#endif