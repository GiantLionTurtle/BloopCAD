
#include "line_abstract.hpp"

#include <glm/gtx/string_cast.hpp>

line_abstract::line_abstract(point_abstract ptA, point_abstract ptB):
	mPointA(point_abstract_ptr(new point_abstract(ptA))),
	mPointB(point_abstract_ptr(new point_abstract(ptB)))
{

}
line_abstract::line_abstract(point_abstract_ptr ptA, point_abstract_ptr ptB):
	mPointA(ptA),
	mPointB(ptB)
{
	
}

XML_element* line_abstract::to_svg(plane_abstract* drawingPlane, glm::vec2 &min, glm::vec2 &max)
{
	glm::vec2 ptA = drawingPlane->point_3d_to_2d(mPointA->pos_vec());
	glm::vec2 ptB = drawingPlane->point_3d_to_2d(mPointB->pos_vec());
	ptA.y = -ptA.y;
	ptB.y = -ptB.y;
	
	update_min_max(ptA, min, max);
	update_min_max(ptB, min, max);

	XML_element* svgLine = new XML_element("line");
	svgLine->add_lastAttribute(new XML_attribute("x1", std::to_string(ptA.x)));
	svgLine->add_lastAttribute(new XML_attribute("y1", std::to_string(ptA.y)));
	svgLine->add_lastAttribute(new XML_attribute("x2", std::to_string(ptB.x)));
	svgLine->add_lastAttribute(new XML_attribute("y2", std::to_string(ptB.y)));
	svgLine->add_lastAttribute(new XML_attribute("stroke", "red"));
	svgLine->add_lastAttribute(new XML_attribute("stroke-width", "1"));
	return svgLine;
}

float line_abstract::dist_to_point(point_abstract const& pt)
{
	return closest_point(pt)->dist(pt);
}
point_abstract_ptr line_abstract::closest_point(point_abstract const& pt)
{
	float len2 = length2();
	if(len2 == 0) {
		return mPointA;
	}
	glm::vec3 line_vec = as_vec();
	float t = glm::dot(line_vec, mPointA->pos_vec()-pt.pos_vec()) / len2;
	if(t <= 0) { // First point
		return mPointA;
	} else if(t >= 1) { // Second point 
		return mPointB;
	} else {
		return at(t);
	}
}
point_abstract_ptr line_abstract::closest_point(point_abstract const& pt, float& on_line)
{
	float len2 = length2();
	if(len2 == 0) {
		return mPointA;
	}
	glm::vec3 line_vec = as_vec();
	on_line = glm::dot(line_vec, mPointA->pos_vec()-pt.pos_vec()) / len2;
	if(on_line <= 0) { // First point
		return mPointA;
	} else if(on_line >= 1) { // Second point 
		return mPointB;
	} else {
		return at(on_line);
	}
}
float line_abstract::length()
{
	return mPointA->dist(mPointB);
}
float line_abstract::length2()
{
	return mPointA->dist2(mPointB);
}

point_abstract_ptr line_abstract::at(float t)
{
	return (mPointA - t * as_vec());
}

glm::vec3 line_abstract::as_vec()
{
	return mPointA->pos_vec() - mPointB->pos_vec();
}