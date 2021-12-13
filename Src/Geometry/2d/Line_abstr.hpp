
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

#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_


#include <Utils/Expunge.hpp>

#include "Point_abstr.hpp"
#include <glm/glm.hpp>

namespace Geom2d {

template<class Impl>
class Line_abstr : public Geom2d_abstr<Line_abstr<Impl>> {
public:
	Line_abstr()
		: Geom2d_abstr<Line_abstr<Impl>>(LINE)
	{

	}

	glm::dvec2 posA()
	{
		return static_cast<Impl*>(this)->posA_impl();
	}
	glm::dvec2 posB()
	{
		return static_cast<Impl*>(this)->posB_impl();
	}
	bool free_impl()
	{
		return static_cast<Impl*>(this)->free_impl2();
	}
	double length()
	{
		return glm::distance(posA(), posB());
	}
	double length2()
	{
		return glm::distance2(posA(), posB());
	}

	glm::dvec2 at_geom(double t)
	{
		return (posA() - t * as_vec());
	}
	bool within_geom(glm::dvec2 top_left, glm::dvec2 bottom_right, bool contained)
	{
		// If the whole line has to be contained, return true if both points are within the selection
		if(contained) {
			return point_within(posA(), top_left, bottom_right) && point_within(posB(), top_left, bottom_right);
		}

		// If only a part has to be contained, return if either point is contained or 
		// if the line crosses  any of the four lines of the selection rectangle
		glm::dvec2 top_right(bottom_right.x, top_left.y), bottom_left(top_left.x, bottom_right.y);
		return point_within(posA(), top_left, bottom_right) || point_within(posB(), top_left, bottom_right)
				|| intersects(top_left, top_right) || intersects(bottom_left, bottom_right)
				|| intersects(top_left, bottom_left) || intersects(top_right, bottom_right);
	}

	double closest_to_point_interp_geom(glm::dvec2 const& pt)
	{
		double len2 = length2();
		if(len2 == 0) {
			return 0.0f;
		}
		double t = glm::dot(as_vec(), posA()-pt) / len2;
		if(t <= 0) { // First point
			return 0.0f;
		} else if(t >= 1) { // Second point 
			return 1.0f;
		} else {
			return t;
		}
	}
	glm::dvec2 closest_to_point_geom(glm::dvec2 const& pt) { return at_geom(closest_to_point_interp_geom(pt)); }
	double dist_to_point_geom(glm::dvec2 const& pt)
	{
		return glm::distance(closest_to_point_geom(pt), pt);
	}

	bool intersects(Line_abstr* l)
	{
		// Better explanation than I could give
		// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
		return ccw(posA(), l->posA(), l->posB()) != ccw(posB(), l->posA(), l->posB()) && ccw(posA(), posB(), l->posA()) != ccw(posA(), posB(), l->posB());
	}
	bool intersects(glm::dvec2 a, glm::dvec2 b)
	{
		// Better explanation than I could give
		// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
		return ccw(posA(), a, b) != ccw(posB(), a, b) && ccw(posA(), posB(), a) != ccw(posA(), posB(), b);
	}
	glm::dvec2 as_vec()
	{
		return posA() - posB();
	}
protected:
	bool ccw(glm::dvec2 A, glm::dvec2 B, glm::dvec2 C) // determine if three point are aranged in a counter clockwise fashion
	{
		return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
	}
};


class Simple_line : public Line_abstr<Simple_line> {
private:
	glm::dvec2 mA, mB;
public:
	Simple_line(glm::dvec2 a, glm::dvec2 b)
		: mA(a)
		, mB(b)
	{
		
	}

	glm::dvec2 posA_impl() { return mA; }
	glm::dvec2 posB_impl() { return mB; }

	bool free_impl2() { return true; }
};

class Line : public Line_abstr<Line> {
private:
	Geom2d::Point* mA, *mB;
public:
	Line(glm::dvec2 a, glm::dvec2 b)
		: Line_abstr<Line>()
		, mA(new Point(a))
		, mB(new Point(b))
	{

	}
	Line()
		: Line_abstr<Line>()
		, mA(nullptr)
		, mB(nullptr)
	{

	}
	// ~Line()
	// {
	// 	expunge(mA);
	// 	expunge(mB);
	// }

	glm::dvec2 posA_impl() { return mA->pos(); }
	glm::dvec2 posB_impl() { return mB->pos(); }

	Geom2d::Point* ptA() { return mA; }
	Geom2d::Point* ptB() { return mB; }

	bool free_impl2() { return mA->free() && mB->free(); }
};

// #include "Line_abstr.cpp"

} // !Geom2d

#endif