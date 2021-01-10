
#include "line_abstract.hpp"

line_abstract::line_abstract(point_abstract ptA, point_abstract ptB):
	mPointA(std::shared_ptr<point_abstract>(new point_abstract(ptA))),
	mPointB(std::shared_ptr<point_abstract>(new point_abstract(ptB)))
{

}
line_abstract::line_abstract(std::shared_ptr<point_abstract> ptA, std::shared_ptr<point_abstract> ptB):
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
	// https://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	return glm::length(glm::cross((pt.pos_vec() - mPointA->pos_vec()), (pt.pos_vec() - mPointB->pos_vec()))) / length();
}
float line_abstract::length()
{
	return glm::length(mPointA->pos_vec() - mPointB->pos_vec());
}