
#ifndef POINT_ABSTR2_HPP_
#define POINT_ABSTR2_HPP_

#include "Geom2d_abstr.hpp"

#include <glm/glm.hpp>

namespace Geom2d {

class Point_abstr : virtual public Geom2d_abstr {
public:
	Point_abstr();
    virtual ~Point_abstr();

	virtual glm::vec2 pos() = 0;
	virtual void set(glm::vec2 p) {}

	virtual glm::vec2 at(float t) { return pos(); }
	bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);
	static bool within(glm::vec2 pt, glm::vec2 top_left, glm::vec2 bottom_right);

	float closest_to_point_interp(glm::vec2 const& pt) { return 0.0f; }
	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) { return pos(); }
	virtual float dist_to_point(glm::vec2 const& pt);
	virtual float dist_to_point_2(glm::vec2 const& pt);
	virtual float dist_to_point_2(Point_abstr& pt) { return dist_to_point_2(pt.pos()); }
	virtual float dist_to_point_2(Point_abstr_ptr pt) { return dist_to_point_2(pt->pos()); }
};

std::ostream& operator<<(std::ostream& os, Point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, Point_abstr& p);

} // !Geom2d

#endif