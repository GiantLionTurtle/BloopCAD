
#include "circle_abstr.hpp"

#include "Line_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace Geom2d {

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

bool circle_abstr::intersects(Line_abstr* l)
{
	// Maths from https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

	glm::vec2 line_dir = l->posB() - l->posA(); // line start to end
	glm::vec2 start_center = l->posA() - posCenter(); // line start to circle center

	// Three params of quadratic equation
	float a = glm::dot(line_dir, line_dir);
	float b = 2 * glm::dot(start_center, line_dir);
	float c = glm::dot(start_center, start_center) - valRadius() * valRadius();

	float determinant = b*b - 4 * a * c;

	if(determinant < 0) { // No solution, it is invalid for sqrt()
		return false; 
	}

	float sqrt_determinant = std::sqrt(determinant);

	float t0 = (-b - sqrt_determinant) / (2*a);

	if(t0 >= 0 && t0 <= 1)
		return true;

	float t1 = (-b + sqrt_determinant) / (2*a);

	if(t1 >= 0 && t1 <= 1) 
		return true;
	return false;
}

} // !Geom2d