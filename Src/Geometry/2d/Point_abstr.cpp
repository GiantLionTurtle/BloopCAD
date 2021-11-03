
#include "Point_abstr.hpp"

namespace Geom2d {

bool point_within(glm::vec2 pt, glm::vec2 top_left, glm::vec2 bottom_right)
{
	if(top_left.x <= pt.x && bottom_right.x >= pt.x && top_left.y >= pt.y && bottom_right.y <= pt.y)
		return true;
	return false;
}

}