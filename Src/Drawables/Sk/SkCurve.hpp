
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

#include <Utils/Expunge.hpp>
#include <Graphics_utils/Camera.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/VertexBufferLayout.hpp>
#include <Graphics_utils/VertexArray.hpp>

#include <vector>

class SkCurve_abstr : public SkPrimitiveGeometry {
public:
	SkCurve_abstr(Geom3d::Plane_abstr* pl, bool fixed_):
		SkPrimitiveGeometry(pl, fixed_)
	{
		mType |= CURVE;
	}

	virtual void set_handle(size_t ind, ExpVec2<ExpVar>* pt) = 0;

	virtual glm::vec2 pos(size_t ind) = 0;
	virtual ExpVec2<ExpVar>* pt(size_t ind) = 0;
	virtual ExpVec2<Expression_abstr> atExp(float t) = 0;

	virtual SelPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual SkExpSelPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
protected:
	virtual void move_impl(glm::vec2 delta) = 0;
};

template<typename Container, typename Derived>
class SkCurve : public SkCurve_abstr {
protected:
	Container mHandles;

	static float kSelDist2;

	VertexArray* mVA;
	VertexBuffer* mVB;
	Shader* mShader;
public:
	SkCurve(Geom3d::Plane_abstr* pl, bool fixed_):
		SkCurve_abstr(pl, fixed_)
	{
		mType |= Drawable_types::CURVE;
	}
	virtual ~SkCurve()
	{
		if(mInited) {
			expunge(mVA);
			expunge(mVB);
			// Shader is not deleted because the shader pool manages it
		}
	}

	void set_handle(size_t ind, ExpVec2<ExpVar>* pt) 
	{
		mHandles.at(ind) = pt;
		set_need_graphicUpdate();
	}

	glm::vec2 pos(size_t ind) { return mHandles.at(ind)->pos(); }
	ExpVec2<ExpVar>* pt(size_t ind) { return mHandles.at(ind); }

	SelPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		if(mType & filter) {
			glm::vec3 worldpos = mBasePlane->to_worldPos(static_cast<Derived*>(this)->closest_to_point(planePos));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
				return { this, glm::distance(cam->pos(), worldpos) };
		}
		return SelPoint();
	}
	SkExpSelPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		if(mType & filter) {
			float t = static_cast<Derived*>(this)->closest_to_point_interp(planePos);
			glm::vec3 worldpos = mBasePlane->to_worldPos(static_cast<Derived*>(this)->at(t));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
				return SkExpSelPoint(this, atExp(t));
		}
		return SkExpSelPoint();
	}
	virtual void release()
	{
		for(int i = 0; i < mHandles.size(); ++i) {
			pt(i)->x()->set_dragged(false);
			pt(i)->y()->set_dragged(false);
		}
	}
	virtual std::vector<var_ptr> all_vars()
	{
		std::vector<var_ptr> out(mHandles.size() * 2);
		for(int i = 0; i < mHandles.size(); ++i) {
			out[2*i] = pt(i)->x();
			out[2*i+1] = pt(i)->y();
		}
		return out;
	}
protected:
	void move_impl(glm::vec2 delta)
	{
		for(size_t i = 0; i < mHandles.size(); ++i) {
			pt(i)->set(pos(i) + delta);
		}
	}
};

#endif