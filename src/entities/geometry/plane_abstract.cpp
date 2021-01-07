
#include "plane_abstract.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

plane_abstract::plane_abstract(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, bool reversed) :
	mOrigin(origin),
	mV(v),
	mW(w),
	mDir(reversed ? -1.0f : 1.0f)
{

}

plane_abstract plane_abstract::from_1Point2Vectors(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, bool reversed)
{
	return plane_abstract(origin, v, w, reversed);
}

std::shared_ptr<plane_abstract> plane_abstract::from_1Point2Vectors_ptr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, bool reversed)
{
	return std::shared_ptr<plane_abstract>(new plane_abstract(origin, v, w, reversed));
}

float plane_abstract::dist(glm::vec3 const& point) const
{
	return std::abs(dist_signed(point));
}
float plane_abstract::dist_signed(glm::vec3 const& point) const
{
	// straight from https://en.wikipedia.org/wiki/Plane_(geometry)

	// Convert to point normal form
	glm::vec3 norm = glm::normalize(glm::cross(mV, mW)) * mDir;

	// Create the parameters of the point normal form
	float a = norm.x;
	float b = norm.y;
	float c = norm.z;
	float d = -(a * mOrigin.x + b * mOrigin.y + c * mOrigin.z);

	// Apply the point normal to point distance formula
	return (a * point.x + b * point.y + c * point.z + d);
}

glm::vec3 plane_abstract::line_intersection(glm::vec3 const& point, glm::vec3 const& vec)
{
	// straight from https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	float t = (glm::dot(glm::cross(mV, mW), (point - mOrigin))) / (glm::dot(-vec, glm::cross(mV, mW)));
	return point + vec * t;
}

glm::vec2 plane_abstract::point_3d_to_2d(glm::vec3 const& point)
{
	glm::vec3 pt_around_origin = point - mOrigin;
	return glm::vec2(glm::dot(mV, pt_around_origin), glm::dot(mW, pt_around_origin));
}
glm::vec3 plane_abstract::point_2d_to_3d(glm::vec2 const& point)
{
	return mOrigin + mV * point.x + mW * point.y;
}