
#include "circle_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace geom_2d {

circle_abstr::circle_abstr(point_abstr_ptr center_, variable_ptr radius_, bool immovable):
	geometry_2d_abstr(3),
	mCenter(center_),
	mRadius(radius_)
{
	if(immovable) {
		mCenter->set_constant();
		mRadius->set_constant();
	}
}
circle_abstr::circle_abstr(point_abstr_ptr center_, float radius_, bool immovable):
	geometry_2d_abstr(3),
	mCenter(center_),
	mRadius(variable_ptr(new variable(radius_)))
{
	if(immovable) {
		mCenter->set_constant();
		mRadius->set_constant();
	}
}
circle_abstr::circle_abstr(glm::vec2 center_, float radius_, bool immovable):
	geometry_2d_abstr(3),
	mCenter(point_abstr_ptr(new point_abstr(glm::vec3(center_, 0.0f), immovable))),
	mRadius(variable_ptr(new variable(radius_)))
{
	if(immovable) {
		mRadius->set_constant();
	}
}
circle_abstr::~circle_abstr()
{
	
}

glm::vec2 circle_abstr::at(float t)
{
	float angle = t / (M_PI * 2);
	return center_vec() + glm::vec2(std::cos(angle), std::sin(angle)) * (float)mRadius->val();
}

glm::vec2 circle_abstr::closest_to_point(glm::vec2 const& pt)
{
	glm::vec2 center_to_point = glm::normalize(pt - center_vec());
	return center_vec() + center_to_point * (float)mRadius->val();  
}

float circle_abstr::dist_to_point(glm::vec2 const& pt)
{
	float dist_to_center = mCenter->dist_to_point(pt);
	if(dist_to_center < mRadius->val()) {
		return mRadius->val() - dist_to_center;
	} else {
		return dist_to_center - mRadius->val();
	}
}

} // !geom_2d