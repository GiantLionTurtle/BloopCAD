
#ifndef SKGEOMETRY_HPP_
#define SKGEOMETRY_HPP_

#include "SkDrawable.hpp"

#include <Geometry/2d/Geom2d_abstr.hpp>
#include "SkSprite.hpp"
#include <Utils/Expunge.hpp>

#include <vector>

class SkGeometry : public SkIntDrawable, public ParamIterator {
protected:
	std::vector<std::pair<SkSprite*, bool>> mAnnots; // Annotation and if it's position has to be updated 
	bool mFixed;
public:
	SkGeometry(Geom3d::Plane_abstr* pl, bool fixed_)
		: SkIntDrawable(pl)
		, mFixed(fixed_)
	{

	}
	virtual ~SkGeometry() {}

	virtual bool closest_2d_draggable(SkGeomDragPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;

	void update_annots()
	{
		for(int i = 0; i < mAnnots.size(); ++i) {
			if(!mAnnots[i].second)
				continue;
			set_annotPos(mAnnots[i].first);
			set_annotOffset(mAnnots[i].first, i+1);
		}
	}

	virtual bool fixed() { return mFixed; }

	virtual void add_annot(SkSprite* sp, bool update_pos = true)
	{
		if(!sp)
			return;
		mAnnots.push_back(std::make_pair(sp, update_pos));
		sp->set_hidden(!selected());

		if(update_pos) {
			set_annotPos(sp);
			set_annotOffset(sp, mAnnots.size());
		}
	}
	virtual void set_annotPos(SkSprite* sp) = 0;
	virtual void set_annotOffset(SkSprite* sp, int ind) = 0;
protected:
	virtual void select_impl(bool sel)
	{
		for(int i = 0; i < mAnnots.size(); ++i) {
			mAnnots[i].first->set_hidden(!sel);
		}		
	}
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
			a.first->set_exists(ex);
		}
	}
};

#endif