
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

#ifndef GEOMETRY_2D_ABSTRACT_HPP_
#define GEOMETRY_2D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace Geom2d {

enum GeomTypes { POINT, LINE, CIRCLE };

template<class Geom>
class Geom2d_abstr {
private:
	GeomTypes mType;
public:
	Geom2d_abstr(GeomTypes type)
		: mType(type) 
	{

	}
	GeomTypes type() { return mType; }

	glm::dvec2 at(double t);

	bool within(glm::dvec2 top_left, glm::dvec2 bottom_right, bool contained);

	double closest_to_point_interp(glm::dvec2 const& pt); // Plugging this function in at(t) should give the same as closest_to_point
	glm::dvec2 closest_to_point(glm::dvec2 const& pt);

	double dist_to_point(glm::dvec2 const& pt);

	bool free();
};

#include "Geom2d_abstr.cpp"

} // !Geom2d

#endif