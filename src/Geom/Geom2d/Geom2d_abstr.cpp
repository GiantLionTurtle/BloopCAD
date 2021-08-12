
#include "Geom2d_abstr.hpp"
#include "Point_abstr.hpp"

namespace Geom2d {

Geom2d_abstr::Geom2d_abstr()
{

}
Geom2d_abstr::~Geom2d_abstr()
{
    
}

glm::vec2 Geom2d_abstr::closest_to_point(Point_abstr& pt)
{
    return closest_to_point(pt.pos());
}

float Geom2d_abstr::dist_to_point(Point_abstr& pt)
{
    return dist_to_point(pt.pos());
}

} // !Geom2d