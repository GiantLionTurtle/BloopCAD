
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

#include "Geom2d_abstr.hpp"
#include "Point_abstr.hpp"

template<typename Geom>
glm::vec2 Geom2d_abstr<Geom>::at(float t)
{
	return static_cast<Geom*>(this)->at_geom(t);
}

template<typename Geom>
bool Geom2d_abstr<Geom>::within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	return static_cast<Geom*>(this)->within_geom(top_left, bottom_right, contained);
}

template<typename Geom>
float Geom2d_abstr<Geom>::closest_to_point_interp(glm::vec2 const& pt)
{
	return static_cast<Geom*>(this)->closest_to_point_interp_geom(pt);
}

template<typename Geom>
glm::vec2 Geom2d_abstr<Geom>::closest_to_point(glm::vec2 const& pt)
{
	return static_cast<Geom*>(this)->closest_to_point_geom(pt);
}

template<typename Geom>
float Geom2d_abstr<Geom>::dist_to_point(glm::vec2 const& pt)
{
    return static_cast<Geom*>(this)->dist_to_point_geom(pt);
}
