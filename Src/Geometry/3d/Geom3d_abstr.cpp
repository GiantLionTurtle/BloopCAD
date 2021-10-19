
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