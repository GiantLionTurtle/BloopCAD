
#ifndef SKCURVE_HPP_
#define SKCURVE_HPP_

#include "SkGeometry.hpp"

#include <vector>

class SkCurve_abstr : public SkPrimitiveGeometry {
public:
	SkCurve_abstr(Geom3d::Plane_abstr* pl, bool fixed_):
		SkPrimitiveGeometry(pl, fixed_)
	{
		mType |= CURVE;
	}

	virtual void set_handle(size_t ind, Geom2d::ExpressionPoint<var_ptr>* pt) = 0;

	virtual glm::vec2 pos(size_t ind) = 0;
	virtual Geom2d::ExpressionPoint<var_ptr>* pt(size_t ind) = 0;

	virtual SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual std::unique_ptr<DraggableSelectionPoint> closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
protected:
	virtual void move_impl(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) = 0;
};

template<typename Container, typename Derived>
class SkCurve : public SkCurve_abstr {
protected:
	Container mHandles;

	static float kSelDist2;

	VertexArray* mVA;
	VertexBuffer* mVB;
	std::shared_ptr<Shader> mShader;
public:
	SkCurve(Geom3d::Plane_abstr* pl, bool fixed_):
		SkCurve_abstr(pl, fixed_)
	{

	}
	~SkCurve()
	{
		delete mVA;
		delete mVB;
	}

	void set_handle(size_t ind, Geom2d::ExpressionPoint<var_ptr>* pt) 
	{
		mHandles.at(ind) = pt;
		set_need_update();
	}

	glm::vec2 pos(size_t ind) { return mHandles.at(ind)->pos(); }
	Geom2d::ExpressionPoint<var_ptr>* pt(size_t ind) { return mHandles.at(ind); }

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		if(mType & filter) {
			glm::vec3 worldpos = mBasePlane->to_worldPos(static_cast<Derived*>(this)->closest_to_point(planePos));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
				return { this, glm::distance(cam->pos(), worldpos) };
		}
		return SelectionPoint();
	}
	std::unique_ptr<DraggableSelectionPoint> closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		if(mType & filter) {
			glm::vec3 worldpos = mBasePlane->to_worldPos(static_cast<Derived*>(this)->closest_to_point(planePos));
			if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
				return std::make_unique<DraggableSelectionPoint>(this);
		}
		return nullptr;
	}
protected:
	void move_impl(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov)
	{
		for(size_t i = 0; i < mHandles.size(); ++i) {
			pt(i)->set(pos(i) + end - start);
		}
	}
};

#endif