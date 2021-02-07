
#include "geometry_2d_abstr.hpp"
#include "point_abstr.hpp"

namespace geom_2d {

geometry_2d_abstr::geometry_2d_abstr()
{

}
geometry_2d_abstr::~geometry_2d_abstr()
{
    
}

glm::vec2 geometry_2d_abstr::closest_to_point(point_abstr& pt)
{
    return closest_to_point(pt.pos());
}

float geometry_2d_abstr::dist_to_point(point_abstr& pt)
{
    return dist_to_point(pt.pos());
}

} // !geom_2d