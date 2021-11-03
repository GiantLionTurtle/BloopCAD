
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

template<class Impl>
class Circle_abstr : public Geom2d_abstr<Circle_abstr<Impl>> {
public:
	Circle_abstr()
		: Geom2d_abstr<Circle_abstr<Impl>>(CIRCLE)
	{

	}
	glm::vec2 center_pos()
	{
		return static_cast<Impl*>(this)->center_pos_impl();
	}
	float radius_val()
	{
		return static_cast<Impl*>(this)->radius_val_impl();
	}

	glm::vec2 at_geom(float t)
	{
		float angle = t * (M_PI * 2.0f);
		return center_pos() + glm::vec2(std::cos(angle), std::sin(angle)) * (float)radius_val();
	}
	bool within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		if(contained) {
			glm::vec2 c = center_pos();
			float r = radius_val();
			return top_left.x <= c.x - r && top_left.y >= c.y + r && bottom_right.x >= c.x + r && bottom_right.y <= c.y - r;
		}
		Geom2d::Simple_line up(top_left, glm::vec2(bottom_right.x, top_left.y)),
									down(glm::vec2(top_left.x, bottom_right.y), bottom_right),
									// right(a, glm::vec2(a.x, b.y)),
									left(glm::vec2(top_left.x, bottom_right.y), top_left);
		return intersects(up) || intersects(down) /*|| intersects(&right)*/ || intersects(left) || within_geom(top_left, bottom_right, true);
	}

	float closest_to_point_interp_geom(glm::vec2 const& pt)
	{
		// Closest point radialy then do inverse of at()
		glm::vec2 pt_norm = glm::normalize(pt - center_pos());
		float angle = std::asin(pt_norm.y);
		return angle / (M_PI * 2.0f);
	}
	glm::vec2 closest_to_point_geom(glm::vec2 const& pt)
	{
		glm::vec2 center_to_point = glm::normalize(pt - center_pos());
		return center_pos() + center_to_point * (float)radius_val();  
	}
	float dist_to_point_geom(glm::vec2 const& pt)
	{
		float dist_to_center = glm::distance2(center_pos(), pt);
		if(dist_to_center < radius_val()) {
			return radius_val() - dist_to_center;
		} else {
			return dist_to_center - radius_val();
		}
	}

	bool intersects(Simple_line& l)
	{
		// Maths from https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

		glm::vec2 line_dir = l.posB() - l.posA(); // line start to end
		glm::vec2 start_center = l.posA() - center_pos(); // line start to circle center

		// Three params of quadratic equation
		float a = glm::dot(line_dir, line_dir);
		float b = 2 * glm::dot(start_center, line_dir);
		float c = glm::dot(start_center, start_center) - radius_val() * radius_val();

		float determinant = b*b - 4 * a * c;

		if(determinant < 0) { // No solution, it is invalid for sqrt()
			return false; 
		}

		float sqrt_determinant = std::sqrt(determinant);

		float t0 = (-b - sqrt_determinant) / (2*a);

		if(t0 >= 0 && t0 <= 1)
			return true;

		float t1 = (-b + sqrt_determinant) / (2*a);

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
	Circle(glm::vec2 c, float r)
		: mCenter(c)
		, mRadius(r)
	{

	}
	Circle()
		: mCenter(glm::vec2(0.0f, 0.0f))
		, mRadius(0.0)
	{

	}

	glm::vec2 center_pos_impl() { return mCenter.pos(); }
	double radius_val_impl() { return mRadius.val(); }

	Geom2d::Point* center() { return &mCenter; }
	Param* radius() { return &mRadius; }
};

// #include "Circle_abstr.cpp"

} // !Geom2d

#endif