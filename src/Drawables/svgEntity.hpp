
#ifndef SVGENTITY_HPP_
#define SVGENTITY_HPP_

#include <utils/xmlParser.hpp>
#include <Geom/Geom3d/Plane_abstr.hpp>

class svgEntity {
public:
	virtual XML_element* to_svg(Geom3d::Plane_abstr* drawingPlane, glm::vec2 &min, glm::vec2 &max) = 0;
protected:
	void update_min_max(glm::vec2 pos, glm::vec2 &min, glm::vec2 &max)
	{
		if(pos.x < min.x) {
			min.x = pos.x;
		} else if(pos.x > max.x) {
			max.x = pos.x;
		}

		if(pos.y < min.y) {
			min.y = pos.y;
		} else if(pos.y > max.y) {
			max.y = pos.y;
		}
	}
};

#endif