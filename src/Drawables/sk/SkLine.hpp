
#ifndef SKLINE_HPP_
#define SKLINE_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Collection.hpp>
#include "SkPoint.hpp"
#include "SkLineCurve.hpp"

#include <glm/glm.hpp>

class SkLine : public SkGeometry, public Collection_abstr<LinearFixed_indexer<SkGeometry*, 3>, true> {
private:
	SkPoint* mPtA, *mPtB;
	SkLineCurve* mCurve;
public:
	SkLine(glm::vec2 ptA, glm::vec2 ptB, Geom3d::plane_abstr* pl, bool fixed_);

	SkPoint* ptA() { return mPtA; }
	SkPoint* ptB() { return mPtB; }
	SkLineCurve* curve() { return mCurve; }

	void notify(Drawable* who, int msg, bool child);

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 3; }
	void add_annot(SkSprite* sp) { mCurve->add_annot(sp); }
	void set_annotPos(SkSprite* sp) { mCurve->set_annotPos(sp); }
	void set_annotOffset(SkSprite* sp, int ind) { mCurve->set_annotOffset(sp, ind); }
protected:
	void select_impl(bool sel);
	
	void move_selected_impl(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov);
};

#endif