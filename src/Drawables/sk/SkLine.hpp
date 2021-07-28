
#ifndef SKLINE_HPP_
#define SKLINE_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Collection.hpp>
#include "SkPoint.hpp"
#include "SkLineCurve.hpp"

#include <glm/glm.hpp>

class SkLine : public SkGeometry, public Linked_Collection<std::array<SkGeometry*, 3>> {
private:
	SkPoint* mPtA, *mPtB;
	SkLineCurve* mCurve;
public:
	SkLine(glm::vec2 ptA, glm::vec2 ptB, geom_3d::plane_abstr* pl, bool fixed_);

	SkPoint* ptA() { return mPtA; }
	SkPoint* ptB() { return mPtB; }
	SkLineCurve* curve() { return mCurve; }

	void notify(Drawable* who, int msg, bool child);

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 3; }
	void move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov);
	void add_annot(SkSprite* sp) { mCurve->add_annot(sp); }
	void set_annotOffset(SkSprite* sp, int ind) { mCurve->set_annotOffset(sp, ind); }
};

#endif