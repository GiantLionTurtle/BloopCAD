
#ifndef GEOMETRY_2D_ABSTRACT_HPP_
#define GEOMETRY_2D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace Geom2d {

class Point_abstr;
class Line_abstr;
class Circle_abstr;
using Point_abstr_ptr = std::shared_ptr<Point_abstr>;
using Line_abstr_ptr = std::shared_ptr<Line_abstr>;
using Circle_abstr_ptr = std::shared_ptr<Circle_abstr>;

class Geom2d_abstr {
public:
	Geom2d_abstr();
	virtual ~Geom2d_abstr();

	virtual glm::vec2 at(float t) = 0;

	virtual bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained) = 0;

	virtual float closest_to_point_interp(glm::vec2 const& pt) = 0; // Plugging this function in at(t) should give the same as closest_to_point
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) { return at(closest_to_point_interp(pt)); }
	virtual glm::vec2 closest_to_point(Point_abstr& pt);
	virtual glm::vec2 closest_to_point(Point_abstr_ptr pt) { return closest_to_point(*pt.get()); }

	virtual float dist_to_point(glm::vec2 const& pt) = 0;
	virtual float dist_to_point(Point_abstr& pt);
	virtual float dist_to_point(Point_abstr_ptr pt) { return dist_to_point(*pt.get()); }
};

} // !Geom2d

#endif