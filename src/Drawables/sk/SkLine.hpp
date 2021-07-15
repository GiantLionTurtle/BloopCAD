
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
	SkLine(glm::vec2 ptA, glm::vec2 ptB, geom_3d::plane_abstr_ptr pl, bool fixed_);

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
};

#endif