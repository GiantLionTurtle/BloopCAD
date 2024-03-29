
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

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

namespace Geom2d {

Line_abstr::Line_abstr()
{

}
Line_abstr::~Line_abstr()
{
	
}

// XML_element* Line_abstract::to_svg(Plane_abstract* drawingPlane, glm::vec2 &min, glm::vec2 &max)
// {
// 	glm::vec2 ptA = drawingPlane->point_3d_to_2d(mPointA->pos_val());
// 	glm::vec2 ptB = drawingPlane->point_3d_to_2d(mPointB->pos_val());
// 	ptA.y = -ptA.y;
// 	ptB.y = -ptB.y;
	
// 	update_min_max(ptA, min, max);
// 	update_min_max(ptB, min, max);

// 	XML_element* svgLine = new XML_element("line");
// 	svgLine->add_lastAttribute(new XML_attribute("x1", std::to_string(ptA.x)));
// 	svgLine->add_lastAttribute(new XML_attribute("y1", std::to_string(ptA.y)));
// 	svgLine->add_lastAttribute(new XML_attribute("x2", std::to_string(ptB.x)));
// 	svgLine->add_lastAttribute(new XML_attribute("y2", std::to_string(ptB.y)));
// 	svgLine->add_lastAttribute(new XML_attribute("stroke", "red"));
// 	svgLine->add_lastAttribute(new XML_attribute("stroke-width", "1"));
// 	return svgLine;
// }

// Point_abstract_ptr Line_abstract::closest_point(Point_abstract const& pt, float& on_line)
// {
// 	float len2 = length2();
// 	if(len2 == 0) {
// 		return mPointA;
// 	}
// 	glm::vec3 line_vec = as_vec();
// 	on_line = glm::dot(line_vec, mPointA->pos_val()-pt.pos_val()) / len2;
// 	if(on_line <= 0) { // First point
// 		return mPointA;
// 	} else if(on_line >= 1) { // Second point 
// 		return mPointB;
// 	} else {
// 		return at(on_line);
// 	}
// }

glm::vec2 Line_abstr::at(float t)
{
	return (posA() - t * as_vec());
}
bool Line_abstr::within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	// If the whole line has to be contained, return true if both points are within the selection
	if(contained) {
		return Point_abstr::within(posA(), top_left, bottom_right) && Point_abstr::within(posB(), top_left, bottom_right);
	}

	// If only a part has to be contained, return if either point is contained or 
	// if the line crosses  any of the four lines of the selection rectangle
	glm::vec2 top_right(bottom_right.x, top_left.y), bottom_left(top_left.x, bottom_right.y);
	return Point_abstr::within(posA(), top_left, bottom_right) || Point_abstr::within(posB(), top_left, bottom_right)
			|| intersects(top_left, top_right) || intersects(bottom_left, bottom_right)
			|| intersects(top_left, bottom_left) || intersects(top_right, bottom_right);
}
float Line_abstr::closest_to_point_interp(glm::vec2 const& pt)
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
float Line_abstr::dist_to_point(glm::vec2 const& pt)
{
	return glm::distance(closest_to_point(pt), pt);
}

float Line_abstr::length()
{
	return glm::distance(posA(), posB());
}
float Line_abstr::length2()
{
	return glm::distance2(posA(), posB());
}

bool Line_abstr::intersects(Line_abstr* l)
{
	// Better explanation than I could give
	// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
	return ccw(posA(), l->posA(), l->posB()) != ccw(posB(), l->posA(), l->posB()) && ccw(posA(), posB(), l->posA()) != ccw(posA(), posB(), l->posB());
}
bool Line_abstr::intersects(glm::vec2 a, glm::vec2 b)
{
	// Better explanation than I could give
	// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
	return ccw(posA(), a, b) != ccw(posB(), a, b) && ccw(posA(), posB(), a) != ccw(posA(), posB(), b);
}

glm::vec2 Line_abstr::as_vec()
{
	return posA() - posB();
}

bool Line_abstr::ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
	return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
}
// bool line_intersect(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D)
// {
// 	return ccw(A,C,D) != ccw(B,C,D) and ccw(A,B,C) != ccw(A,B,D);
// }

simple_line::simple_line(glm::vec2 a, glm::vec2 b):
	mA(a),
	mB(b)
{
	
}

} // !Geom2d