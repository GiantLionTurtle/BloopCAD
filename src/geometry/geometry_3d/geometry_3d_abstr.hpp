
#ifndef GEOMETRY_3D_ABSTRACT_HPP_
#define GEOMETRY_3D_ABSTRACT_HPP_

#include <memory>

#include <glm/glm.hpp>

namespace geom_3d {

class point_abstr;
class line_abstr;
class plane_abstr;
using point_abstr_ptr = std::shared_ptr<point_abstr>;
using line_abstr_ptr = std::shared_ptr<line_abstr>;
using plane_abstr_ptr = std::shared_ptr<plane_abstr>;

class geometry_3d_abstr {
public:
	geometry_3d_abstr();
	virtual ~geometry_3d_abstr();

	virtual glm::vec3 closest_to_point(glm::vec3 const& pt) = 0;
	virtual glm::vec3 closest_to_point(point_abstr const& pt);
	virtual glm::vec3 closest_to_point(point_abstr_ptr pt) { return closest_to_point(*pt.get()); }

	virtual float dist_to_point(glm::vec3 const& pt) = 0;
	virtual float dist_to_point(point_abstr const& pt);
	virtual float dist_to_point(point_abstr_ptr pt) { return dist_to_point(*pt.get()); }
};

} // !geom_3d

#endif