
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

#ifndef SKCURVE_HPP_
#define SKCURVE_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Base/Collection.hpp>

#include "SkPoint.hpp"
#include <Utils/Expunge.hpp>
#include <Graphics_utils/Camera.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/VertexBufferLayout.hpp>
#include <Graphics_utils/VertexArray.hpp>

#include <vector>

template<typename HI, typename MC> // Handles Indexer & Mathematical Curve
class SkCurve : public SkPrimitiveGeometry<MC>  {
protected:
	float kSelDist2;
	Collection_abstr<HI> mInnerCollection;
public:
	SkCurve(Geom3d::Plane_abstr* pl, bool fixed_, MC* mathcurve):
		SkPrimitiveGeometry<MC>(pl, fixed_, mathcurve)
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