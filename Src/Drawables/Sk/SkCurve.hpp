
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
	SkCurve(Geom3d::Plane_abstr* pl, bool fixed_, MC* mathcurve)
		: SkPrimitiveGeometry<MC>(pl, fixed_, mathcurve)
	{
		kSelDist2 = 25;
		this->mType |= Drawable_types::CURVE;
	}

	void init()
	{
		mInnerCollection.init();
		for(int i = 0; i < n_handles(); ++i) {
			handle(i)->set_parent(this);
		}
		Drawable::init();
	}
	void draw(Camera* cam, int frame, Drawable::draw_type type = Drawable::draw_type::ALL)
	{
		Drawable::draw(cam, frame, type);
		mInnerCollection.draw(cam, frame, type);
	}
	void internalUpdate()
	{
		mInnerCollection.internalUpdate();
		Drawable::internalUpdate();
	}
	void graphicUpdate(bool force = false)
	{
		Drawable::graphicUpdate(force);
		mInnerCollection.graphicUpdate(force);
	}

	void notify(Drawable* who, int msg, bool child)
	{
		if(msg == SkDrawable::SkNotifiers::SET) {
			Drawable::set_need_graphicUpdate(true);
			mInnerCollection.set_need_graphicUpdate(true);
		}
	}

	int n_handles() { return mInnerCollection.drawList().size(); }
	SkPoint*& handle(size_t ind) { return mInnerCollection.drawList().at(ind); }


	bool closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		for(int i = 0; i < n_handles(); ++i) {
			if(handle(i)->closest_2d(selP, planePos, cam, cursorPos, filter))
				return true;
		}
		if(this->mType & filter) {
			glm::vec3 worldpos = this->mBasePlane->to_worldPos(this->mGeom->closest_to_point(planePos));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2) {
				selP.ent = this;
				selP.dist_to_cam = glm::distance(cam->pos(), worldpos);
				return true;
			}
		}
		return false;
	}	
	bool closest_2d_draggable(SkGeomDragPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		for(int i = 0; i < n_handles(); ++i) {
			if(handle(i)->closest_2d_draggable(selP, planePos, cam, cursorPos, filter))
				return true;
		}
		if(this->type() & filter) {
			float t = this->mGeom->closest_to_point_interp(planePos);
			glm::vec3 worldpos = this->mBasePlane->to_worldPos(this->mGeom->at(t));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2) {
				selP.geom = this;
				return true;
			}
		}
		return false;
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
	void move_selected(glm::vec2 delta)
	{
		if(Drawable::selected()) {
			for(int i = 0; i < n_handles(); ++i) {
				handle(i)->move(delta);
			}
		} else {
			for(int i = 0; i < n_handles(); ++i) {
				if(handle(i)->selected())
					handle(i)->move(delta);
			}
		}
	}
protected:
	void select_impl(bool sel)
	{
		SkGeometry::select_impl(sel);
		if(!sel) {
			for(int i = 0; i < n_handles(); ++i) {
				handle(i)->unselect();
			}
		}
	}
};

#endif