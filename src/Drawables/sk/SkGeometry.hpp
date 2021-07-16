
#ifndef SKGEOMETRY_HPP_
#define SKGEOMETRY_HPP_

#include "SkDrawable.hpp"

#include <geometry/geometry_2d/Geom2d_abstr.hpp>

class SkGeometry : public SkDrawable {
protected:
	// Annotation stuff n all?
	bool mFixed;
public:
	SkGeometry(geom_3d::plane_abstr_ptr pl, bool fixed_):
		SkDrawable(pl),
		mFixed(fixed_)
	{

	}

	virtual bool fixed() { return mFixed; }
};

class SkPrimitiveGeometry : public SkGeometry, virtual public Geom2d::Geom2d_abstr {
public:
	SkPrimitiveGeometry(geom_3d::plane_abstr_ptr pl, bool fixed_):
		SkGeometry(pl, fixed_)
	{

	}

	void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		bool should_select = within(top_left, bottom_right, contained);

		if(should_select != selected())
			set_selected(should_select);
	}
};

#endif