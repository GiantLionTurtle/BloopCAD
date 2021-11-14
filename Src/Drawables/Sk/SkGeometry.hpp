
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SKGEOMETRY_HPP_
#define SKGEOMETRY_HPP_

#include "SkDrawable.hpp"

#include <Geometry/2d/Geom2d_abstr.hpp>
#include "SkSprite.hpp"
#include <Utils/Expunge.hpp>

#include <vector>

class SkGeometry : public SkIntDrawable, public ParamIterator {
protected:
	std::vector<SkSprite*> mAnnots;
	bool mFixed;
public:
	SkGeometry(Geom3d::Plane_abstr* pl, bool fixed_)
		: SkIntDrawable(pl)
		, mFixed(fixed_)
	{

	}
	virtual ~SkGeometry() {}

	virtual bool closest_2d_draggable(SkGeomDragPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;

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

template<typename Geom>
class SkPrimitiveGeometry : public SkGeometry {
protected:
	Geom* mGeom;

	VertexArray* mVA;
	VertexBuffer* mVB;
	Shader* mShader;
public:
	SkPrimitiveGeometry(Geom3d::Plane_abstr* pl, bool fixed_, Geom* gm)
		: SkGeometry(pl, fixed_)
		, mGeom(gm)
		, mVA(nullptr)
		, mVB(nullptr)
		, mShader(nullptr)
	{

	}
	SkPrimitiveGeometry()
		: SkGeometry(nullptr, false)
		, mGeom(nullptr)
		, mVA(nullptr)
		, mVB(nullptr)
		, mShader(nullptr)
	{
		
	}
	~SkPrimitiveGeometry()
	{
		expunge(mGeom); // By this point it might already been expunged.. but it matters not delete on nullptr is fine
		expunge(mVA);
		expunge(mVB);
	}
	inline Geom* geom() { return mGeom; }

	void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		bool should_select = mGeom->within(top_left, bottom_right, contained);

		if(should_select != selected())
			set_selected(should_select);
	}
protected:
	void exists_impl(bool ex)
	{
		for(auto a : mAnnots) {
			a->set_exists(ex);
		}
	}
};

#endif