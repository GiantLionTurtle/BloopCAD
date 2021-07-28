
#ifndef POINT_ABSTR3_HPP_
#define POINT_ABSTR3_HPP_

#include "geometry_3d_abstr.hpp"

#include <glm/glm.hpp>

namespace Geom3d {

class Point_abstr : public geometry_3d_abstr {
public:
	Point_abstr();
    virtual ~Point_abstr();

	virtual glm::vec3 pos() const = 0;
	virtual void set(glm::vec3 const& p) = 0;

	virtual glm::vec3 at(float t) { return pos(); }

	virtual glm::vec3 closest_to_point(glm::vec3 const& pt) { return pos(); }
	virtual float dist_to_point(glm::vec3 const& pt);
	virtual float dist_to_point_2(glm::vec3 const& pt);
	virtual float dist_to_point_2(Point_abstr& pt) { return dist_to_point_2(pt.pos()); }
	virtual float dist_to_point_2(Point_abstr_ptr pt) { return dist_to_point_2(pt->pos()); }
protected:
	virtual void post_set_behavior() {}
};

std::ostream& operator<<(std::ostream& os, Point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, Point_abstr& p);

} // !Geom3d

#endif