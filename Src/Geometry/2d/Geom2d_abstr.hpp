
#ifndef GEOMETRY_2D_ABSTRACT_HPP_
#define GEOMETRY_2D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace Geom2d {

enum GeomTypes { POINT, LINE, CIRCLE };

template<class Geom>
class Geom2d_abstr {
private:
	GeomTypes mType;
public:
	Geom2d_abstr(GeomTypes type) : mType(type) {}
	GeomTypes type() { return mType; }

	glm::vec2 at(float t);

	bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp(glm::vec2 const& pt); // Plugging this function in at(t) should give the same as closest_to_point
	glm::vec2 closest_to_point(glm::vec2 const& pt);

	float dist_to_point(glm::vec2 const& pt);
};

#include "Geom2d_abstr.cpp"

} // !Geom2d

#endif