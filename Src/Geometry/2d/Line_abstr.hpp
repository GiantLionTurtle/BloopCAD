
#ifndef LINE_ABSTR2_HPP_
#define LINE_ABSTR2_HPP_

#include <ConstrsSolver/Expression.hpp>

#include "Point_abstr.hpp"
#include <glm/glm.hpp>

namespace Geom2d {

template<class Impl>
class Line_abstr : public Geom2d_abstr<Line_abstr<Impl>> {
public:
	Line_abstr()
		: Geom2d_abstr<Line_abstr<Impl>>(LINE)
	{

	}

	glm::vec2 posA()
	{
		return static_cast<Impl*>(this)->posA_impl();
	}
	glm::vec2 posB()
	{
		return static_cast<Impl*>(this)->posB_impl();
	}
	float length()
	{
		return glm::distance(posA(), posB());
	}
	float length2()
	{
		return glm::distance2(posA(), posB());
	}

	glm::vec2 at_geom(float t)
	{
		return (posA() - t * as_vec());
	}
	bool within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		// If the whole line has to be contained, return true if both points are within the selection
		if(contained) {
			return point_within(posA(), top_left, bottom_right) && point_within(posB(), top_left, bottom_right);
		}

		// If only a part has to be contained, return if either point is contained or 
		// if the line crosses  any of the four lines of the selection rectangle
		glm::vec2 top_right(bottom_right.x, top_left.y), bottom_left(top_left.x, bottom_right.y);
		return point_within(posA(), top_left, bottom_right) || point_within(posB(), top_left, bottom_right)
				|| intersects(top_left, top_right) || intersects(bottom_left, bottom_right)
				|| intersects(top_left, bottom_left) || intersects(top_right, bottom_right);
	}

	float closest_to_point_interp_geom(glm::vec2 const& pt)
	{
		float len2 = length2();
		if(len2 == 0) {
			return 0.0f;
		}
		float t = glm::dot(as_vec(), posA()-pt) / len2;
		if(t <= 0) { // First point
			return 0.0f;
		} else if(t >= 1) { // Second point 
			return 1.0f;
		} else {
			return t;
		}
	}
	glm::vec2 closest_to_point_geom(glm::vec2 const& pt) { return at_geom(closest_to_point_interp_geom(pt)); }
	float dist_to_point_geom(glm::vec2 const& pt)
	{
		return glm::distance(closest_to_point_geom(pt), pt);
	}

	bool intersects(Line_abstr* l)
	{
		// Better explanation than I could give
		// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
		return ccw(posA(), l->posA(), l->posB()) != ccw(posB(), l->posA(), l->posB()) && ccw(posA(), posB(), l->posA()) != ccw(posA(), posB(), l->posB());
	}
	bool intersects(glm::vec2 a, glm::vec2 b)
	{
		// Better explanation than I could give
		// https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
		return ccw(posA(), a, b) != ccw(posB(), a, b) && ccw(posA(), posB(), a) != ccw(posA(), posB(), b);
	}
protected:
	bool ccw(glm::vec2 A, glm::vec2 B, glm::vec2 C) // determine if three point are aranged in a counter clockwise fashion
	{
		return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
	}
	glm::vec2 as_vec()
	{
		return posA() - posB();
	}
};


class Simple_line : public Line_abstr<Simple_line> {
private:
	glm::vec2 mA, mB;
public:
	Simple_line(glm::vec2 a, glm::vec2 b)
		: mA(a)
		, mB(b)
	{
		
	}

	glm::vec2 posA_impl() { return mA; }
	glm::vec2 posB_impl() { return mB; }
};

class Line : public Line_abstr<Line> {
private:
	Geom2d::Point* mA, *mB;
	double* mParams[4];
public:
	Line(glm::vec2 a, glm::vec2 b)
		: Line_abstr<Line>()
		, mA(new Point(a))
		, mB(new Point(b))
	{

	}
	Line()
		: Line_abstr<Line>()
		, mA(nullptr)
		, mB(nullptr)
	{

	}

	glm::vec2 posA_impl() { return mA->pos(); }
	glm::vec2 posB_impl() { return mB->pos(); }

	Geom2d::Point* ptA() { return mA; }
	Geom2d::Point* ptB() { return mB; }
};

// #include "Line_abstr.cpp"

} // !Geom2d

#endif