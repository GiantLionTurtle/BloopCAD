
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

class Point;
class Line;
class Circle;

enum GeomTypes { POINT, LINE, CIRCLE };

template<typename Geom>
class Geom2d_abstr {
private:
	GeomTypes mType;
public:
	Geom2d_abstr(GeomTypes type);
	GeomTypes type() { return mType; }

	glm::vec2 at(float t);

	bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp(glm::vec2 const& pt); // Plugging this function in at(t) should give the same as closest_to_point
	glm::vec2 closest_to_point(glm::vec2 const& pt);

	float dist_to_point(glm::vec2 const& pt);
};

#include "Geom2d_abstr.cpp"

} // !Geom2d

#endif