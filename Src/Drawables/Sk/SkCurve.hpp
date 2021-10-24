
#ifndef SKCURVE_HPP_
#define SKCURVE_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Base/Collection.hpp>

#include "SkPoint.hpp"

#include <Utils/Expunge.hpp>

#include <vector>

template<typename HI, typename MC> // Handles Indexer & Mathematical Curve
class SkCurve : virtual public Collection_abstr<HI>, virtual public SkPrimitiveGeometry<MC>  {
protected:
	static float kSelDist2;
public:
	SkCurve(Geom3d::Plane_abstr* pl, bool fixed_):
		SkPrimitiveGeometry<MC>(pl, fixed_)
	{
		this->mType |= Drawable_types::CURVE;
	}

	int n_handles() { return this->mDrawList.size(); }
	SkPoint*& handle(size_t ind) { return this->mDrawList.at(ind); }

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
		Collection_abstr<HI>::select_within(top_left, bottom_right, contained);
		SkPrimitiveGeometry<MC>::select_within(top_left, bottom_right, contained);
	}
	virtual void release()
	{
		for(int i = 0; i < n_handles(); ++i) {
			auto h_tmp = handle(i);
			h_tmp->x()->set_dragged(false);
			h_tmp->y()->set_dragged(false);
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