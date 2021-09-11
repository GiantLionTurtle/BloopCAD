
#include "Point_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

namespace Geom2d {

Point_abstr::Point_abstr()
{

}
Point_abstr::~Point_abstr()
{

}

bool Point_abstr::within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	return within(pos(), top_left, bottom_right);
}
bool Point_abstr::within(glm::vec2 pt, glm::vec2 top_left, glm::vec2 bottom_right)
{
	if(top_left.x <= pt.x && bottom_right.x >= pt.x && top_left.y >= pt.y && bottom_right.y <= pt.y)
		return true;
	return false;
}

float Point_abstr::dist_to_point(glm::vec2 const& pt)
{
	return glm::distance(pos(), pt);
}
float Point_abstr::dist_to_point_2(glm::vec2 const& pt)
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

} // !Geom2d