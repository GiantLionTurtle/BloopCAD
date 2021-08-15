
#include "Circle_abstr.hpp"

#include "Line_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace Geom2d {

Circle_abstr::Circle_abstr()
{

}
Circle_abstr::~Circle_abstr()
{
	
}

glm::vec2 Circle_abstr::at(float t)
{
	float angle = t * (M_PI * 2.0f);
	return center_pos() + glm::vec2(std::cos(angle), std::sin(angle)) * (float)radius_val();
}
bool Circle_abstr::within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	if(contained) {
		glm::vec2 c = center_pos();
		float r = radius_val();
		return top_left.x <= c.x - r && top_left.y >= c.y + r && bottom_right.x >= c.x + r && bottom_right.y <= c.y - r;
	}
	Geom2d::simple_line up(top_left, glm::vec2(bottom_right.x, top_left.y)),
								down(glm::vec2(top_left.x, bottom_right.y), bottom_right),
								// right(a, glm::vec2(a.x, b.y)),
								left(glm::vec2(top_left.x, bottom_right.y), top_left);
	return intersects(&up) || intersects(&down) /*|| intersects(&right)*/ || intersects(&left) || within(top_left, bottom_right, true);
}
float Circle_abstr::closest_to_point_interp(glm::vec2 const& pt)
{
	// Closest point radialy then do inverse of at()
	glm::vec2 pt_norm = glm::normalize(pt - center_pos());
	float angle = std::asin(pt_norm.y);
	return angle / (M_PI * 2.0f);
}
glm::vec2 Circle_abstr::closest_to_point(glm::vec2 const& pt)
{
	glm::vec2 center_to_point = glm::normalize(pt - center_pos());
	return center_pos() + center_to_point * (float)radius_val();  
}

float Circle_abstr::dist_to_point(glm::vec2 const& pt)
{
	float dist_to_center = glm::distance2(center_pos(), pt);
	if(dist_to_center < radius_val()) {
		return radius_val() - dist_to_center;
	} else {
		return dist_to_center - radius_val();
	}
}

bool Circle_abstr::intersects(Line_abstr* l)
{
	// Maths from https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

	glm::vec2 line_dir = l->posB() - l->posA(); // line start to end
	glm::vec2 start_center = l->posA() - center_pos(); // line start to circle center

	// Three params of quadratic equation
	float a = glm::dot(line_dir, line_dir);
	float b = 2 * glm::dot(start_center, line_dir);
	float c = glm::dot(start_center, start_center) - radius_val() * radius_val();

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