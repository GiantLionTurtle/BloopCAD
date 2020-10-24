
#include "plane_abstract.hpp"

plane_abstract::plane_abstract(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w) :
    mOrigin(origin),
    mV(v),
    mW(w)
{

}

plane_abstract plane_abstract::from_1Point2Vectors(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w)
{
    return plane_abstract(origin, v, w);
}

std::shared_ptr<plane_abstract> plane_abstract::from_1Point2Vectors_ptr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w)
{
    return std::shared_ptr<plane_abstract>(new plane_abstract(origin, v, w));
}

float plane_abstract::dist(glm::vec3 const& point) const
{
    return std::abs(dist_signed(point));
}
float plane_abstract::dist_signed(glm::vec3 const& point) const
{
	glm::vec3 norm = glm::cross(mV, mW);
	float a = norm.x;
	float b = norm.y;
	float c = norm.z;
	float d = -(a * mOrigin.x + b * mOrigin.x + c * mOrigin.z);
	return (a * point.x + b * point.y + c * point.z + d) / std::sqrt(a*a + b*b + c*c);
}