
#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "Point_abstr.hpp"

#include <memory>

namespace Geom2d {

class circle_abstr : public Geom2d_abstr {
public:
	circle_abstr();
	virtual ~circle_abstr();

	virtual glm::vec2 posCenter() = 0;
	virtual float valRadius() = 0;

	virtual glm::vec2 at(float t);

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	bool intersects(Line_abstr* l);
	bool intersects(std::shared_ptr<Line_abstr> l) { return intersects(l.get()); }
};

} // !Geom2d

#endif