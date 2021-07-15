
#ifndef SKGEOMETRY_HPP_
#define SKGEOMETRY_HPP_

#include "SkDrawable.hpp"

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

#endif