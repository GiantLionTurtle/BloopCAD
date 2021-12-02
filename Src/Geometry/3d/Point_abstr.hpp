
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

#ifndef POINT_ABSTR3_HPP_
#define POINT_ABSTR3_HPP_

#include "Geom3d_abstr.hpp"

#include <glm/glm.hpp>

namespace Geom3d {

class Point_abstr : public Geom3d_abstr {
public:
	Point_abstr();
    virtual ~Point_abstr();

	virtual glm::vec3 pos() const = 0;
	virtual void set(glm::vec3 const& p) = 0;

	virtual glm::vec3 at(float t) { return pos(); }

	virtual glm::vec3 closest_to_point(glm::vec3 const& pt) { return pos(); }
	virtual float dist_to_point(glm::vec3 const& pt);
	virtual float dist_to_point_2(glm::vec3 const& pt);
	virtual float dist_to_point_2(Point_abstr& pt) { return dist_to_point_2(pt.pos()); }
	virtual float dist_to_point_2(Point_abstr_ptr pt) { return dist_to_point_2(pt->pos()); }
protected:
	virtual void post_set_behavior() {}
};

std::ostream& operator<<(std::ostream& os, Point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, Point_abstr& p);

} // !Geom3d

#endif