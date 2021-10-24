
#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_

#include <ConstraintsSolver/Expression.hpp>

#include "Point_abstr.hpp"
#include <glm/glm.hpp>

namespace Geom2d {

template<typename Impl>
class Line_abstr : public Geom2d_abstr<Line_abstr<Impl>> {
public:
	Line_abstr();

	glm::vec2 posA();
	glm::vec2 posB();
	float length();
	float length2();

	glm::vec2 at_geom(float t);
	bool within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	float closest_to_point_interp_geom(glm::vec2 const& pt);
	glm::vec2 closest_to_point_geom(glm::vec2 const& pt) { return at_geom(closest_to_point_interp_geom(pt)); }
	float dist_to_point_geom(glm::vec2 const& pt);

	bool intersects(Line_abstr* l);
	bool intersects(glm::vec2 a, glm::vec2 b);
protected:
	bool ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C); // determine if three point are aranged in a counter clockwise fashion
	glm::vec2 as_vec();
};


class Simple_line : public Line_abstr<Simple_line> {
private:
	glm::vec2 mA, mB;
public:
	Simple_line(glm::vec2 a, glm::vec2 b);

	glm::vec2 posA_impl() { return mA; }
	glm::vec2 posB_impl() { return mB; }
};

class Line : public Line_abstr<Simple_line> {
private:
	Geom2d::Point* mA, *mB;
	double* mParams[4];
public:
	Line(glm::vec2 a, glm::vec2 b);
	Line();

	glm::vec2 posA_impl() { return mA->pos(); }
	glm::vec2 posB_impl() { return mB->pos(); }

	Geom2d::Point* ptA() { return mA; }
	Geom2d::Point* ptB() { return mB; }

	int n_params() { return 4; }
	double** params() { return &mParams[0]; }
private:
	void init_params();
};

#include "Line_abstr.cpp"

} // !Geom2d

#endif