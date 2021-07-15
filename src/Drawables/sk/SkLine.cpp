
#include "SkLine.hpp"

SkLine::SkLine(glm::vec2 ptA, glm::vec2 ptB, geom_3d::plane_abstr_ptr pl, bool fixed_):
	SkGeometry(pl, fixed_),
	mPtA(new SkPoint(ptA, pl, fixed_)),
	mPtB(new SkPoint(ptB, pl, fixed_)),
	mCurve(nullptr)
{
	mCurve = new SkLineCurve(mPtA, mPtB, pl, fixed_); // Created within bracket because it relies on the creation of the points
	mDrawList[0] = mPtA;
	mDrawList[1] = mPtB;
	mDrawList[2] = mCurve;
}

SelectionPoint SkLine::closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	for(int i = 0; i < 3; ++i) {
		// it is known that the points come before the curve in mDrawList,
		// because the curve has a lower selection rank, return as soon as 
		// a point is valid 
		SelectionPoint selPt = mDrawList.at(i)->closest_2d(planePos, cam, cursorPos, filter);
		if(selPt.ent)
			return selPt;
	}
	return SelectionPoint();
}
DraggableSelectionPoint SkLine::closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	for(int i = 0; i < 3; ++i) {
		// it is known that the points come before the curve in mDrawList,
		// because the curve has a lower selection rank, return as soon as 
		// a point is valid 
		DraggableSelectionPoint selPt = mDrawList.at(i)->closest_2d_draggable(planePos, cam, cursorPos, filter);
		if(selPt.ent)
			return selPt;
	}
	return DraggableSelectionPoint();
}