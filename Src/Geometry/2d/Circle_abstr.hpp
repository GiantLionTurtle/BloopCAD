
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

#include <memory>

namespace Geom2d {

class Circle_abstr : public virtual Geom2d_abstr {
public:
	Circle_abstr();
	virtual ~Circle_abstr();

	virtual glm::vec2 center_pos() = 0;
	virtual float radius_val() = 0;

	virtual glm::vec2 at(float t);
	virtual bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp(glm::vec2 const& pt);
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	bool intersects(Line_abstr* l);
	bool intersects(std::shared_ptr<Line_abstr> l) { return intersects(l.get()); }
};

} // !Geom2d

#endif