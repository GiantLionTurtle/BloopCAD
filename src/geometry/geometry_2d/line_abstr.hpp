
#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_

#include "point_abstr.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace geom_2d {

class line_abstr : public geometry_2d_abstr {
public:
	line_abstr();
	virtual ~line_abstr();

	virtual glm::vec2 posA() = 0;
	virtual glm::vec2 posB() = 0;

	virtual glm::vec2 at(float t);
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	float length();
	float length2();

	bool intersects(line_abstr* l);
	bool intersects(std::shared_ptr<line_abstr> l) { return intersects(l.get()); }
protected:
	virtual void post_set_update() {}

	bool ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C);
	glm::vec2 as_vec();
};


// bool line_intersect(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);

class simple_line : public line_abstr {
private:
	glm::vec2 mA, mB;
public:
	simple_line(glm::vec2 a, glm::vec2 b);

	virtual glm::vec2 posA() { return mA; }
	virtual glm::vec2 posB() { return mB; }
};

} // !geom_2ds

#endif