
#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_

#include "Point_abstr.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace Geom2d {

class Line_abstr : public virtual Geom2d_abstr {
public:
	Line_abstr();
	virtual ~Line_abstr();

	virtual glm::vec2 posA() = 0;
	virtual glm::vec2 posB() = 0;

	glm::vec2 at(float t);
	bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	float length();
	float length2();

	bool intersects(Line_abstr* l);
protected:
	bool ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C);
	glm::vec2 as_vec();
};


// bool line_intersect(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);

class simple_line : public Line_abstr {
private:
	glm::vec2 mA, mB;
public:
	simple_line(glm::vec2 a, glm::vec2 b);

	virtual glm::vec2 posA() { return mA; }
	virtual glm::vec2 posB() { return mB; }
};

} // !Geom2ds

#endif