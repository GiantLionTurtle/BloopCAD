
#ifndef SKLINE_HPP_
#define SKLINE_HPP_

#include "SkHandleCurve.hpp"
#include "SkLineCurve.hpp"

#include <glm/glm.hpp>

class SkLine : public SkHandleCurve<SkHandleCurve_FixedIndexer<2>, SkLineCurve> {
public:
	SkLine(glm::vec2 ptA, glm::vec2 ptB, Geom3d::plane_abstr* pl, bool fixed_);

	SkPoint* ptA() { return handle(0); }
	SkPoint* ptB() { return handle(1); }

	int selection_rank() { return 3; }
};

#endif