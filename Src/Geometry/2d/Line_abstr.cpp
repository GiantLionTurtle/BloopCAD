
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

#include "Line_abstr.hpp"
#include "Point_abstr.hpp"

template<typename Impl>
Line_abstr<Impl>::Line_abstr()
	: Geom2d_abstr<Line_abstr<Impl>>(LINE)
{

}

template<typename Impl>
glm::vec2 Line_abstr<Impl>::posA()
{
	return static_cast<Impl*>(this)->posA_impl();
}
template<typename Impl>
glm::vec2 Line_abstr<Impl>::posB()
{
	return static_cast<Impl*>(this)->posB_impl();
}
template<typename Impl>
float Line_abstr<Impl>::length()
{
	return glm::distance(posA(), posB());
}
template<typename Impl>
float Line_abstr<Impl>::length2()
{
	return glm::distance2(posA(), posB());
}

template<typename Impl>
glm::vec2 Line_abstr<Impl>::at_geom(float t)
{
	return (posA() - t * as_vec());
}

template<typename Impl>
bool Line_abstr<Impl>::within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	// If the whole line has to be contained, return true if both points are within the selection
	if(contained) {
		return point_within(posA(), top_left, bottom_right) && point_within(posB(), top_left, bottom_right);
	}

	// If only a part has to be contained, return if either point is contained or 
	// if the line crosses  any of the four lines of the selection rectangle
	glm::vec2 top_right(bottom_right.x, top_left.y), bottom_left(top_left.x, bottom_right.y);
	return point_within(posA(), top_left, bottom_right) || point_within(posB(), top_left, bottom_right)
			|| intersects(top_left, top_right) || intersects(bottom_left, bottom_right)
			|| intersects(top_left, bottom_left) || intersects(top_right, bottom_right);
}
template<typename Impl>
float Line_abstr<Impl>::closest_to_point_interp_geom(glm::vec2 const& pt)
{
	float len2 = length2();
	if(len2 == 0) {
		return 0.0f;
	}
	float t = glm::dot(as_vec(), posA()-pt) / len2;
	if(t <= 0) { // First point
		return 0.0f;
	} else if(t >= 1) { // Second point 
		return 1.0f;
	} else {
		return t;
	}
}
template<typename Impl>
float Line_abstr<Impl>::dist_to_point_geom(glm::vec2 const& pt)
{
	return glm::distance(closest_to_point_geom(pt), pt);
}
template<typename Impl>
bool Line_abstr<Impl>::intersects(Line_abstr* l)
{
	// Better explanation than I could give
	// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
	return ccw(posA(), l->posA(), l->posB()) != ccw(posB(), l->posA(), l->posB()) && ccw(posA(), posB(), l->posA()) != ccw(posA(), posB(), l->posB());
}
template<typename Impl>
bool Line_abstr<Impl>::intersects(glm::vec2 a, glm::vec2 b)
{
	// Better explanation than I could give
	// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
	return ccw(posA(), a, b) != ccw(posB(), a, b) && ccw(posA(), posB(), a) != ccw(posA(), posB(), b);
}

template<typename Impl>
glm::vec2 Line_abstr<Impl>::as_vec()
{
	return posA() - posB();
}
template<typename Impl>
bool Line_abstr<Impl>::ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
	return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
}


Line::Line(glm::vec2 a, glm::vec2 b)
	: mA(new Point(a))
	, mB(new Point(b))
{
	init_params();
}
Line::Line()
	: mA(nullptr)
	, mB(nullptr)
{
	init_params();
}

void Line::init_params()
{
	mParams[0] = mA->x_raw();
	mParams[1] = mA->y_raw();
	mParams[2] = mB->x_raw();
	mParams[3] = mB->y_raw();
}