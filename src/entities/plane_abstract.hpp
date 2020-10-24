
#ifndef PLANE_ABSTRACT_HPP_
#define PLANE_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

class plane_abstract {
protected:
	glm::vec3 mOrigin, mV, mW;
public:
	plane_abstract(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w);

	// static plane_abstract from_1Point1Vector(glm::vec3 origin, glm::vec3 n);
	static plane_abstract from_1Point2Vectors(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w); // this one is just a wrapper around the basic constructor
	// static plane_abstract from_3Points(glm::vec3 origin, glm::vec3 pt1, glm::vec3 pt2);

	// static std::shared_ptr<plane_abstract> from_1Point1Vector_ptr(glm::vec3 origin, glm::vec3 n);
	static std::shared_ptr<plane_abstract> from_1Point2Vectors_ptr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w); // this one is just a wrapper around the basic constructor
	// static std::shared_ptr<plane_abstract> from_3Points_ptr(glm::vec3 origin, glm::vec3 pt1, glm::vec3 pt2);

	float dist(glm::vec3 const& point) const;
	float dist_signed(glm::vec3 const& point) const;
};

#endif