
#ifndef GEOMETRY_2D_ABSTRACT_HPP_
#define GEOMETRY_2D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace geom_2d {

class point_abstr;
class line_abstr;
class circle_abstr;
using point_abstr_ptr = std::shared_ptr<point_abstr>;
using line_abstr_ptr = std::shared_ptr<line_abstr>;
using circle_abstr_ptr = std::shared_ptr<circle_abstr>;

class geometry_2d_abstr {
public:
	geometry_2d_abstr();
	virtual ~geometry_2d_abstr();

	virtual glm::vec2 at(float t) = 0;

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) = 0;
	virtual glm::vec2 closest_to_point(point_abstr& pt);
	virtual glm::vec2 closest_to_point(point_abstr_ptr pt) { return closest_to_point(*pt.get()); }

	virtual float dist_to_point(glm::vec2 const& pt) = 0;
	virtual float dist_to_point(point_abstr& pt);
	virtual float dist_to_point(point_abstr_ptr pt) { return dist_to_point(*pt.get()); }
};

} // !geom_2d

#endif