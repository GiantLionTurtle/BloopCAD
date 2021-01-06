
#include "line_abstract.hpp"

line_abstract::line_abstract(point_abstract ptA, point_abstract ptB)
{
	mPointA = new point_abstract(ptA);
    mPointB = new point_abstract(ptB);
}
line_abstract::line_abstract(point_abstract* ptA, point_abstract* ptB):
	mPointA(ptA),
	mPointB(ptB)
{
	
}

XML_element* line_abstract::to_svg(plane_abstract* drawingPlane, glm::vec2 &min, glm::vec2 &max)
{
	glm::vec2 ptA = drawingPlane->point_on_plane(mPointA->pos());
	glm::vec2 ptB = drawingPlane->point_on_plane(mPointB->pos());
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