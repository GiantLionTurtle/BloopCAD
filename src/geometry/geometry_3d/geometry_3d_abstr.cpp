
#include "geometry_3d_abstr.hpp"
#include "Point_abstr.hpp"

namespace geom_3d {

geometry_3d_abstr::geometry_3d_abstr()
{

}
geometry_3d_abstr::~geometry_3d_abstr()
{

}
glm::vec3 geometry_3d_abstr::closest_to_point(Point_abstr const& pt)
{
    return closest_to_point(pt.pos());
}

float geometry_3d_abstr::dist_to_point(Point_abstr const& pt)
{
    return dist_to_point(pt.pos());
}

} // !geom_3d