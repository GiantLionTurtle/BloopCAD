
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

#ifndef GEOMETRY_3D_ABSTRACT_HPP_
#define GEOMETRY_3D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace Geom3d {

class Point_abstr;
class Line_abstr;
class Plane_abstr;
using Point_abstr_ptr = std::shared_ptr<Point_abstr>;
using Line_abstr_ptr = std::shared_ptr<Line_abstr>;
using Plane_abstr_ptr = std::shared_ptr<Plane_abstr>;

class Geom3d_abstr {
public:
	Geom3d_abstr();
	virtual ~Geom3d_abstr();

	virtual glm::vec3 closest_to_point(glm::vec3 const& pt) = 0;
	virtual glm::vec3 closest_to_point(Point_abstr const& pt);
	virtual glm::vec3 closest_to_point(Point_abstr_ptr pt) { return closest_to_point(*pt.get()); }

	virtual float dist_to_point(glm::vec3 const& pt) = 0;
	virtual float dist_to_point(Point_abstr const& pt);
	virtual float dist_to_point(Point_abstr_ptr pt) { return dist_to_point(*pt.get()); }
};

} // !Geom3d

#endif