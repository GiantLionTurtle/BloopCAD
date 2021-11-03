
#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "Point_abstr.hpp"
#include "Line_abstr.hpp"

#include <memory>

namespace Geom2d {

template<typename Impl>
class Circle_abstr : public Geom2d_abstr<Circle_abstr<Impl>> {
public:
	Circle_abstr();
	glm::vec2 center_pos();
	float radius_val();

	glm::vec2 at_geom(float t);
	bool within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp_geom(glm::vec2 const& pt);
	glm::vec2 closest_to_point_geom(glm::vec2 const& pt);
	float dist_to_point_geom(glm::vec2 const& pt);

	bool intersects(Simple_line& l);
};

class Circle : public Circle_abstr<Circle> {
private:
	Geom2d::Point mCenter;
	Param mRadius;
public:
	Circle(glm::vec2 c, float r);
	Circle();

	glm::vec2 center_pos_impl() { return mCenter.pos(); }
	double radius_val_impl() { return mRadius.val(); }

	Geom2d::Point* center() { return &mCenter; }
	Param* radius() { return &mRadius; }
};

#include "Circle_abstr.cpp"

} // !Geom2d

#endif