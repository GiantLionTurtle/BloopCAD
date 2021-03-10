
#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "point_abstr.hpp"

#include <memory>

namespace geom_2d {

class circle_abstr : public geometry_2d_abstr {
public:
	circle_abstr();
	virtual ~circle_abstr();

	virtual glm::vec2 posCenter() = 0;
	virtual float valRadius() = 0;

	virtual glm::vec2 at(float t);

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	bool intersects(line_abstr* l);
	bool intersects(std::shared_ptr<line_abstr> l) { return intersects(l.get()); }
};

} // !geom_2d

#endif