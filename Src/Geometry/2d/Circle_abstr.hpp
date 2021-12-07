
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

#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "Point_abstr.hpp"
#include "Line_abstr.hpp"

#include <memory>

namespace Geom2d {

template<class Impl>
class Circle_abstr : public Geom2d_abstr<Circle_abstr<Impl>> {
public:
	Circle_abstr()
		: Geom2d_abstr<Circle_abstr<Impl>>(CIRCLE)
	{

	}
	glm::dvec2 center_pos()
	{
		return static_cast<Impl*>(this)->center_pos_impl();
	}
	double radius_val()
	{
		return static_cast<Impl*>(this)->radius_val_impl();
	}

	glm::dvec2 at_geom(double t)
	{
		double angle = t * (M_PI * 2.0f);
		return center_pos() + glm::dvec2(std::cos(angle), std::sin(angle)) * (double)radius_val();
	}
	bool within_geom(glm::dvec2 top_left, glm::dvec2 bottom_right, bool contained)
	{
		if(contained) {
			glm::dvec2 c = center_pos();
			double r = radius_val();
			return top_left.x <= c.x - r && top_left.y >= c.y + r && bottom_right.x >= c.x + r && bottom_right.y <= c.y - r;
		}
		Geom2d::Simple_line up(top_left, glm::dvec2(bottom_right.x, top_left.y)),
									down(glm::dvec2(top_left.x, bottom_right.y), bottom_right),
									// right(a, glm::dvec2(a.x, b.y)),
									left(glm::dvec2(top_left.x, bottom_right.y), top_left);
		return intersects(up) || intersects(down) /*|| intersects(&right)*/ || intersects(left) || within_geom(top_left, bottom_right, true);
	}

	double closest_to_point_interp_geom(glm::dvec2 const& pt)
	{
		// Closest point radialy then do inverse of at()
		glm::dvec2 pt_norm = glm::normalize(pt - center_pos());
		double angle = std::asin(pt_norm.y);
		return angle / (M_PI * 2.0f);
	}
	glm::dvec2 closest_to_point_geom(glm::dvec2 const& pt)
	{
		glm::dvec2 center_to_point = glm::normalize(pt - center_pos());
		return center_pos() + center_to_point * (double)radius_val();  
	}
	double dist_to_point_geom(glm::dvec2 const& pt)
	{
		double dist_to_center = glm::distance2(center_pos(), pt);
		if(dist_to_center < radius_val()) {
			return radius_val() - dist_to_center;
		} else {
			return dist_to_center - radius_val();
		}
	}

	bool intersects(Simple_line& l)
	{
		// Maths from https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

		glm::dvec2 line_dir = l.posB() - l.posA(); // line start to end
		glm::dvec2 start_center = l.posA() - center_pos(); // line start to circle center

		// Three params of quadratic equation
		double a = glm::dot(line_dir, line_dir);
		double b = 2 * glm::dot(start_center, line_dir);
		double c = glm::dot(start_center, start_center) - radius_val() * radius_val();

		double determinant = b*b - 4 * a * c;

		if(determinant < 0) { // No solution, it is invalid for sqrt()
			return false; 
		}

		double sqrt_determinant = std::sqrt(determinant);

		double t0 = (-b - sqrt_determinant) / (2*a);

		if(t0 >= 0 && t0 <= 1)
			return true;

		double t1 = (-b + sqrt_determinant) / (2*a);

		if(t1 >= 0 && t1 <= 1) 
			return true;
		return false;
	}
};

class Circle : public Circle_abstr<Circle> {
private:
	Geom2d::Point mCenter;
	Param mRadius;
public:
	Circle(glm::dvec2 c, double r)
		: mCenter(c)
		, mRadius(r)
	{

	}
	Circle()
		: mCenter(glm::dvec2(0.0f, 0.0f))
		, mRadius(0.0)
	{

	}

	glm::dvec2 center_pos_impl() { return mCenter.pos(); }
	double radius_val_impl() { return mRadius.val(); }

	Geom2d::Point* center() { return &mCenter; }
	Param* radius() { return &mRadius; }
};

// #include "Circle_abstr.cpp"

} // !Geom2d

#endif