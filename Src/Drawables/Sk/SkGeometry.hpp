
#ifndef SKGEOMETRY_HPP_
#define SKGEOMETRY_HPP_

#include "SkDrawable.hpp"

#include <Geometry/2d/Geom2d_abstr.hpp>
#include "SkSprite.hpp"

#include <vector>

class SkGeometry : public SkIntDrawable {
protected:
	std::vector<SkSprite*> mAnnots;
	bool mFixed;
public:
	SkGeometry(Geom3d::Plane_abstr* pl, bool fixed_):
		SkIntDrawable(pl),
		mFixed(fixed_)
	{

	}
	virtual ~SkGeometry() {}

	void move_selected(glm::vec2 delta)
	{
		if(!fixed())
			move_selected_impl(delta);
	}
	void move(glm::vec2 delta) 
	{
		if(!fixed())
			move_impl(delta);
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

	virtual void move_selected_impl(glm::vec2 delta) { if(selected()) move(delta); }
	virtual void move_impl(glm::vec2 delta) {}
};

class SkPrimitiveGeometry : public SkGeometry, virtual public Geom2d::Geom2d_abstr {
public:
	SkPrimitiveGeometry(Geom3d::Plane_abstr* pl, bool fixed_):
		SkGeometry(pl, fixed_)
	{

	}
	virtual ~SkPrimitiveGeometry() {}

	void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		bool should_select = within(top_left, bottom_right, contained);

		if(should_select != selected())
			set_selected(should_select);
	}
protected:
	void exists_impl(bool ex)
	{
		for(auto v : all_vars()) {
			v->set_exists(ex);
		}
		for(auto a : mAnnots) {
			a->set_exists(ex);
		}
	}
};

#endif