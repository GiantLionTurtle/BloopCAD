
#include "circle_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace geom_2d {

circle_abstr::circle_abstr()
{

}
circle_abstr::~circle_abstr()
{
	
}

glm::vec2 circle_abstr::at(float t)
{
	float angle = t * (M_PI * 2);
	return posCenter() + glm::vec2(std::cos(angle), std::sin(angle)) * (float)valRadius();
}

glm::vec2 circle_abstr::closest_to_point(glm::vec2 const& pt)
{
	glm::vec2 center_to_point = glm::normalize(pt - posCenter());
	return posCenter() + center_to_point * (float)valRadius();  
}

float circle_abstr::dist_to_point(glm::vec2 const& pt)
{
	float dist_to_center = glm::distance2(posCenter(), pt);
	if(dist_to_center < valRadius()) {
		return valRadius() - dist_to_center;
	} else {
		return dist_to_center - valRadius();
	}
}

} // !geom_2d