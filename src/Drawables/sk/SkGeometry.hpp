
#ifndef SKGEOMETRY_HPP_
#define SKGEOMETRY_HPP_

#include "SkDrawable.hpp"

#include <geometry/geometry_2d/Geom2d_abstr.hpp>
#include "SkSprite.hpp"

#include <vector>

class SkGeometry : public SkDrawable {
protected:
	std::vector<SkSprite*> mAnnots;
	bool mFixed;
public:
	SkGeometry(geom_3d::plane_abstr_ptr pl, bool fixed_):
		SkDrawable(pl),
		mFixed(fixed_)
	{

	}

	void update_annots()
	{
		for(int i = 0; i < mAnnots.size(); ++i) {
			set_annotOffset(mAnnots[i], i+1);
		}
	}

	virtual bool fixed() { return mFixed; }

	virtual void add_annot(SkSprite* sp)
	{
		if(!sp) 
			return;
		mAnnots.push_back(sp);
		set_annotOffset(sp, mAnnots.size());
	}
	virtual void set_annotOffset(SkSprite* sp, int ind) = 0;
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