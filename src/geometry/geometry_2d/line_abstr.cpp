
#include "line_abstr.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

namespace geom_2d {

line_abstr::line_abstr()
{

}
line_abstr::~line_abstr()
{
	
}

// XML_element* line_abstract::to_svg(plane_abstract* drawingPlane, glm::vec2 &min, glm::vec2 &max)
// {
// 	glm::vec2 ptA = drawingPlane->point_3d_to_2d(mPointA->pos_val());
// 	glm::vec2 ptB = drawingPlane->point_3d_to_2d(mPointB->pos_val());
// 	ptA.y = -ptA.y;
// 	ptB.y = -ptB.y;
	
// 	update_min_max(ptA, min, max);
// 	update_min_max(ptB, min, max);

// 	XML_element* svgLine = new XML_element("line");
// 	svgLine->add_lastAttribute(new XML_attribute("x1", std::to_string(ptA.x)));
// 	svgLine->add_lastAttribute(new XML_attribute("y1", std::to_string(ptA.y)));
// 	svgLine->add_lastAttribute(new XML_attribute("x2", std::to_string(ptB.x)));
// 	svgLine->add_lastAttribute(new XML_attribute("y2", std::to_string(ptB.y)));
// 	svgLine->add_lastAttribute(new XML_attribute("stroke", "red"));
// 	svgLine->add_lastAttribute(new XML_attribute("stroke-width", "1"));
// 	return svgLine;
// }

// point_abstract_ptr line_abstract::closest_point(point_abstract const& pt, float& on_line)
// {
// 	float len2 = length2();
// 	if(len2 == 0) {
// 		return mPointA;
// 	}
// 	glm::vec3 line_vec = as_vec();
// 	on_line = glm::dot(line_vec, mPointA->pos_val()-pt.pos_val()) / len2;
// 	if(on_line <= 0) { // First point
// 		return mPointA;
// 	} else if(on_line >= 1) { // Second point 
// 		return mPointB;
// 	} else {
// 		return at(on_line);
// 	}
// }

glm::vec2 line_abstr::at(float t)
{
	return (posA() - t * as_vec());
}
glm::vec2 line_abstr::closest_to_point(glm::vec2 const& pt)
{
	float len2 = length2();
	if(len2 == 0) {
		return posA();
	}
	float t = glm::dot(as_vec(), posA()-pt) / len2;
	if(t <= 0) { // First point
		return posA();
	} else if(t >= 1) { // Second point 
		return posB();
	} else {
		return at(t);
	}
}
float line_abstr::dist_to_point(glm::vec2 const& pt)
{
	return glm::distance(closest_to_point(pt), pt);
}

float line_abstr::length()
{
	return glm::distance(posA(), posB());
}
float line_abstr::length2()
{
	return glm::distance2(posA(), posB());
}

glm::vec2 line_abstr::as_vec()
{
	return posA() - posB();
}

simple_line::simple_line(glm::vec2 a, glm::vec2 b):
	mA(a),
	mB(b)
{
	
}

} // !geom_2d