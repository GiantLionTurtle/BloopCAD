
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

#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_

#include "Point_abstr.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace Geom2d {

class Line_abstr : public virtual Geom2d_abstr {
public:
	Line_abstr();
	virtual ~Line_abstr();

	virtual glm::vec2 posA() = 0;
	virtual glm::vec2 posB() = 0;

	glm::vec2 at(float t);
	bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	float length();
	float length2();

	bool intersects(Line_abstr* l);
	bool intersects(glm::vec2 a, glm::vec2 b);
protected:
	bool ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C); // determine if three point are aranged in a counter clockwise fashion
	glm::vec2 as_vec();
};


// bool line_intersect(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);

class simple_line : public Line_abstr {
private:
	glm::vec2 mA, mB;
public:
	simple_line(glm::vec2 a, glm::vec2 b);

	virtual glm::vec2 posA() { return mA; }
	virtual glm::vec2 posB() { return mB; }
};

} // !Geom2ds

#endif