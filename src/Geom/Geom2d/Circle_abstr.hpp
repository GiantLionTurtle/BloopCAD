
#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "Point_abstr.hpp"

#include <memory>

namespace Geom2d {

class Circle_abstr : public virtual Geom2d_abstr {
public:
	Circle_abstr();
	virtual ~Circle_abstr();

	virtual glm::vec2 center_pos() = 0;
	virtual float radius_val() = 0;

	virtual glm::vec2 at(float t);
	virtual bool within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained);

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);

	bool intersects(Line_abstr* l);
	bool intersects(std::shared_ptr<Line_abstr> l) { return intersects(l.get()); }
};

} // !Geom2d

#endif