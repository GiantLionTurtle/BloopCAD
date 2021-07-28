
#include "SkLine.hpp"

SkLine::SkLine(glm::vec2 ptA, glm::vec2 ptB, Geom3d::plane_abstr* pl, bool fixed_):
	SkGeometry(pl, fixed_),
	mPtA(new SkPoint(ptA, pl, fixed_)),
	mPtB(new SkPoint(ptB, pl, fixed_)),
	mCurve(nullptr)
{
	mCurve = new SkLineCurve(mPtA, mPtB, pl, fixed_); // Created within bracket because it relies on the creation of the points
	mDrawList[0] = mPtA;
	mDrawList[1] = mPtB;
	mDrawList[2] = mCurve;

	set_name("SkLine");
}

void SkLine::notify(Drawable* who, int msg, bool child)
{
	if(!child || who != mCurve)
		return;

	switch(msg) {
	case RESURRECTED:
		set_exists(true);
		break;
	case DELETED:
		set_exists(false);
		break;
	case SELECTED:
		set_selected(true);
		break;
	case UNSELECTED:
		set_selected(false);
		break;
	}
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
void SkLine::move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov)
{
	// All this logic to prevent the line to move two times (if the curve moves then the points)
	if(mCurve->selected()) {
		mCurve->move(start, end, pix_mov);
	} else {
		if(mPtA->selected())
			mPtA->move(start, end, pix_mov);
		if(mPtB->selected())
			mPtB->move(start, end, pix_mov);
	}
}