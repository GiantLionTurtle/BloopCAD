
#include "point_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

namespace geom_2d {

point_abstr::point_abstr()
{

}
point_abstr::~point_abstr()
{

}

float point_abstr::dist_to_point(glm::vec2 const& pt)
{
	return glm::distance(pos(), pt);
}
float point_abstr::dist_to_point_2(glm::vec2 const& pt)
{
	return glm::distance2(pos(), pt);
}

std::ostream& operator<<(std::ostream& os, point_abstr_ptr p)
{
	os<<*p.get();
	return os;
}
std::ostream& operator<<(std::ostream& os, point_abstr& p)
{
	os<<glm::to_string(p.pos());
	return os;
}

} // !geom_2d