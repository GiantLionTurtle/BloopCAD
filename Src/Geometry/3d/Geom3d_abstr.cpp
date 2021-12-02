
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

#include "Geom3d_abstr.hpp"
#include "Point_abstr.hpp"

namespace Geom3d {

Geom3d_abstr::Geom3d_abstr()
{

}
Geom3d_abstr::~Geom3d_abstr()
{

}
glm::vec3 Geom3d_abstr::closest_to_point(Point_abstr const& pt)
{
    return closest_to_point(pt.pos());
}

float Geom3d_abstr::dist_to_point(Point_abstr const& pt)
{
    return dist_to_point(pt.pos());
}

} // !Geom3d