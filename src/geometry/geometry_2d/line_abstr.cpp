
#include "line_abstr.hpp"

#include <glm/gtx/string_cast.hpp>

namespace geom_2d {

line_abstr::line_abstr(glm::vec2 ptA, glm::vec2 ptB):
	geometry_2d_abstr(2),
	mA(point_abstr_ptr(new point_abstr(ptA))),
	mB(point_abstr_ptr(new point_abstr(ptB)))
{

}
line_abstr::line_abstr(point_abstr ptA, point_abstr ptB):
	geometry_2d_abstr(2),
	mA(point_abstr_ptr(new point_abstr(ptA))),
	mB(point_abstr_ptr(new point_abstr(ptB)))
{

}
line_abstr::line_abstr(point_abstr_ptr ptA, point_abstr_ptr ptB):
	geometry_2d_abstr(2),
	mA(ptA),
	mB(ptB)
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
	return (A()->vec() - t * as_vec());
}
glm::vec2 line_abstr::closest_to_point(glm::vec2 const& pt)
{
	float len2 = length2();
	if(len2 == 0) {
		return mA->vec();
	}
	glm::vec2 line_vec = as_vec();
	float t = glm::dot(line_vec, mA->vec()-pt) / len2;
	if(t <= 0) { // First point
		return mA->vec();
	} else if(t >= 1) { // Second point 
		return mB->vec();
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
	return mA->dist_to_point(mB->vec());
}
float line_abstr::length2()
{
	return mA->dist_to_point_2(mB);
}

glm::vec2 line_abstr::as_vec()
{
	return mA->vec() - mB->vec();
}

} // !geom_2d