
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
	SkGeometry(Geom3d::plane_abstr* pl, bool fixed_):
		SkDrawable(pl),
		mFixed(fixed_)
	{

	}

	void update_annots()
	{
		for(int i = 0; i < mAnnots.size(); ++i) {
			set_annotPos(mAnnots[i]);
			set_annotOffset(mAnnots[i], i+1);
		}
	}

	virtual bool fixed() { return mFixed; }

	virtual void add_annot(SkSprite* sp)
	{
		if(!sp) 
			return;
		mAnnots.push_back(sp);
		sp->set_hidden(!selected());
		
		set_annotPos(sp);
		set_annotOffset(sp, mAnnots.size());
	}
	virtual void set_annotPos(SkSprite* sp) = 0;
	virtual void set_annotOffset(SkSprite* sp, int ind) = 0;
protected:
	virtual void select_impl(bool sel)
	{
		for(int i = 0; i < mAnnots.size(); ++i) {
			mAnnots[i]->set_hidden(!sel);
		}		
	}
};

class SkPrimitiveGeometry : public SkGeometry, virtual public Geom2d::Geom2d_abstr {
public:
	SkPrimitiveGeometry(Geom3d::plane_abstr* pl, bool fixed_):
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