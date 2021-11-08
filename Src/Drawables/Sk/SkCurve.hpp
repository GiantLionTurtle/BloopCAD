
#ifndef SKCURVE_HPP_
#define SKCURVE_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Base/Collection.hpp>

#include "SkPoint.hpp"

#include <Utils/Expunge.hpp>

#include <vector>

template<typename HI, typename MC> // Handles Indexer & Mathematical Curve
class SkCurve : public SkPrimitiveGeometry<MC>  {
protected:
	float kSelDist2;
	Collection_abstr<HI> mInnerCollection;
public:
	SkCurve(Geom3d::Plane_abstr* pl, bool fixed_):
		SkPrimitiveGeometry<MC>(pl, fixed_)
	{
		kSelDist2 = 25;
		this->mType |= Drawable_types::CURVE;
	}

	void init()
	{
		mInnerCollection.init();
		Drawable::init();
	}
	void draw_impl(Camera* cam, int frame, Drawable::draw_type type = Drawable::draw_type::ALL)
	{
		mInnerCollection.draw(cam, frame, type);
		Drawable::draw(cam, frame, type);
	}
	void internalUpdate_impl()
	{
		mInnerCollection.internalUpdate();
		Drawable::internalUpdate();
	}
	void graphicUpdate_impl()
	{
		mInnerCollection.graphicUpdate();
		Drawable::graphicUpdate();
	}

	int n_handles() { return mInnerCollection.drawList().size(); }
	SkPoint*& handle(size_t ind) { return mInnerCollection.drawList().at(ind); }

	SelPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		if(this->mType & filter) {
			glm::vec3 worldpos = this->mBasePlane->to_worldPos(this->mGeom->closest_to_point(planePos));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
				return { this, glm::distance(cam->pos(), worldpos) };
		}
		return SelPoint();
	}	
	SkExpSelPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		if(this->type() & filter) {
			float t = this->mGeom->closest_to_point_interp(planePos);
			glm::vec3 worldpos = this->mBasePlane->to_worldPos(this->mGeom->at(t));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
				return SkExpSelPoint(this);//, atExp(t));
		}
		return SkExpSelPoint();
	}
	void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		mInnerCollection.select_within(top_left, bottom_right, contained);
		SkPrimitiveGeometry<MC>::select_within(top_left, bottom_right, contained);
	}
	virtual void release()
	{
		for(int i = 0; i < n_handles(); ++i) {
			auto h_tmp = handle(i);
			h_tmp->x()->set_frozen(false);
			h_tmp->y()->set_frozen(false);
		}
	}
	virtual std::vector<var_ptr> all_vars()
	{
		// std::vector<var_ptr> out(mHandles.size() * 2);
		// for(int i = 0; i < mHandles.size(); ++i) {
		// 	out[2*i] = pt(i)->x();
		// 	out[2*i+1] = pt(i)->y();
		// }
		// return out;
		return {};
	}
protected:
	void move_impl(glm::vec2 delta)
	{
		for(int i = 0; i < n_handles(); ++i) {
			handle(i)->move(delta);
		}
	}
};

#endif