
#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_

#include "point_abstr.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace geom_2d {

class line_abstr : public geometry_2d_abstr {
protected:
	point_abstr_ptr mA, mB;
public:
	line_abstr(glm::vec2 ptA, glm::vec2 ptB);
	line_abstr(point_abstr ptA, point_abstr ptB);
	line_abstr(point_abstr_ptr ptA, point_abstr_ptr ptB);
	virtual ~line_abstr();

	point_abstr_ptr A() const { return mA; }
	point_abstr_ptr B() const { return mB; }

	virtual glm::vec2 at(float t);
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	float length();
	float length2();
protected:
	virtual void post_set_update() {}

	glm::vec2 as_vec();
};

} // !geom_2ds

#endif