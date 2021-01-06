
#ifndef LINE_ABSTRACT_HPP_
#define LINE_ABSTRACT_HPP_

#include "point_abstract.hpp"
#include "../svgEntity.hpp"

#include <glm/glm.hpp>
#include <iostream>

/*
	@class line_abstract is an abstract representation of a line
*/
class line_abstract : public svgEntity {
protected:
	point_abstract* mPointA, *mPointB; // Both points that describe the line in 3d space
public:
	/*
		@line_abstract creates a line_abstract

		@param ptA : The start point of the line
		@param ptB : The end point of the line
	*/
	line_abstract(point_abstract ptA, point_abstract ptB);
	line_abstract(point_abstract* ptA, point_abstract* ptB);

	/*
		@function pointA is a getter for the start point of the line

		@return : The start point of the line
	*/
	point_abstract pointA() const { return *mPointA; }
	virtual void set_pointA(point_abstract ptA) { mPointA->set_pos(ptA); }
	/*
		@function pointB is a getter for the end point of the line

		@return : The end point of the line
	*/
	point_abstract pointB() const { return *mPointB; }
	virtual void set_pointB(point_abstract ptB) { mPointB->set_pos(ptB); }

	virtual XML_element* to_svg(plane_abstract* drawingPlane, glm::vec2 &min, glm::vec2 &max);
};

#endif