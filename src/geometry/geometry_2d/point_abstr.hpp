
#ifndef POINT_ABSTR2_HPP_
#define POINT_ABSTR2_HPP_

#include "geometry_2d_abstr.hpp"

#include <glm/glm.hpp>

namespace geom_2d {

class point_abstr : public geometry_2d_abstr {
public:
	point_abstr();
    virtual ~point_abstr();

	virtual glm::vec2 pos() const = 0;
	virtual void set(glm::vec2 p) = 0;

	virtual glm::vec2 at(float t) { return pos(); }

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) { return pos(); }
	virtual float dist_to_point(glm::vec2 const& pt);
	virtual float dist_to_point_2(glm::vec2 const& pt);
	virtual float dist_to_point_2(point_abstr& pt) { return dist_to_point_2(pt.pos()); }
	virtual float dist_to_point_2(point_abstr_ptr pt) { return dist_to_point_2(pt->pos()); }
protected:
	virtual void post_set_behavior() {}
};

std::ostream& operator<<(std::ostream& os, point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, point_abstr& p);

} // !geom_2d

#endif