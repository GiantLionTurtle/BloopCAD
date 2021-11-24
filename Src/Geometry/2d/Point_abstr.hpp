
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

#ifndef POINT_ABSTR2_HPP_
#define POINT_ABSTR2_HPP_

#include "Geom2d_abstr.hpp"

#include <glm/glm.hpp>

namespace Geom2d {

class Point_abstr : virtual public Geom2d_abstr {
public:
	Point_abstr();
    virtual ~Point_abstr();

	virtual glm::vec2 pos() = 0;
	virtual void set(glm::vec2 p) {}

	virtual glm::vec2 at(float t) { return pos(); }
	bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);
	static bool within(glm::vec2 pt, glm::vec2 top_left, glm::vec2 bottom_right);

	float closest_to_point_interp(glm::vec2 const& pt) { return 0.0f; }
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) { return pos(); }
	virtual float dist_to_point(glm::vec2 const& pt);
	virtual float dist_to_point_2(glm::vec2 const& pt);
	virtual float dist_to_point_2(Point_abstr& pt) { return dist_to_point_2(pt.pos()); }
	virtual float dist_to_point_2(Point_abstr_ptr pt) { return dist_to_point_2(pt->pos()); }
};

std::ostream& operator<<(std::ostream& os, Point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, Point_abstr& p);

} // !Geom2d

#endif