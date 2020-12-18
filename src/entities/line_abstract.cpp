
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
