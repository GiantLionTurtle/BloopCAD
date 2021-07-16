
#include "Point_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

namespace geom_3d {

Point_abstr::Point_abstr()
{

}
Point_abstr::~Point_abstr()
{

}

float Point_abstr::dist_to_point(glm::vec3 const& pt)
{
	return glm::distance(pos(), pt);
}
float Point_abstr::dist_to_point_2(glm::vec3 const& pt)
{
	return glm::distance2(pos(), pt);
}

std::ostream& operator<<(std::ostream& os, Point_abstr_ptr p)
{
	os<<*p.get();
	return os;
}
std::ostream& operator<<(std::ostream& os, Point_abstr& p)
{
	os<<glm::to_string(p.pos());
	return os;
}

} // !geom_3d