
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

#ifndef GEOMETRY_2D_ABSTRACT_HPP_
#define GEOMETRY_2D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace Geom2d {

class Point_abstr;
class Line_abstr;
class Circle_abstr;
using Point_abstr_ptr = std::shared_ptr<Point_abstr>;
using Line_abstr_ptr = std::shared_ptr<Line_abstr>;
using Circle_abstr_ptr = std::shared_ptr<Circle_abstr>;

class Geom2d_abstr {
public:
	Geom2d_abstr();
	virtual ~Geom2d_abstr();

	virtual glm::vec2 at(float t) = 0;

	virtual bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained) = 0;

	virtual float closest_to_point_interp(glm::vec2 const& pt) = 0; // Plugging this function in at(t) should give the same as closest_to_point
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) { return at(closest_to_point_interp(pt)); }
	virtual glm::vec2 closest_to_point(Point_abstr& pt);
	virtual glm::vec2 closest_to_point(Point_abstr_ptr pt) { return closest_to_point(*pt.get()); }

	virtual float dist_to_point(glm::vec2 const& pt) = 0;
	virtual float dist_to_point(Point_abstr& pt);
	virtual float dist_to_point(Point_abstr_ptr pt) { return dist_to_point(*pt.get()); }
};

} // !Geom2d

#endif