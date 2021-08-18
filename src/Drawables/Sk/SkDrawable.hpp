
#ifndef SKETCHDRAWABLE_HPP_
#define SKETCHDRAWABLE_HPP_

#include <Geom/Geom3d/Plane_abstr.hpp>
#include <Drawables/Tangible.hpp>
#include <ConstraintsSolver/Expression.hpp>
#include <ConstraintsSolver/ExpressionVec.hpp>

class SkDrawable;

struct SkExpSelPoint {
	SkExpSelPoint():
		ent(nullptr)
	{

	}
	SkExpSelPoint(SkDrawable* dr, ExpVec2<Expression> p):
		ent(dr),
		pt(p)
	{
		
	}

	operator bool() { return ent; }

	SkDrawable* ent;
	ExpVec2<Expression> pt;
};

class SkDrawable : virtual public Drawable {
protected:
	Geom3d::Plane_abstr* mBasePlane; // Should it be the parent sketch directly??
public:
	SkDrawable(Geom3d::Plane_abstr* pl):
		mBasePlane(pl)
	{
		mType |= Drawable_types::SKDRAWABLE;
	}

	Geom3d::Plane_abstr* basePlane() { return mBasePlane; }
	void set_basePlane(Geom3d::Plane_abstr* pl) { mBasePlane = pl; set_need_update(); }

	virtual SelPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual SkExpSelPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual void move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) { if(selected()) move(start, end, pix_mov); }
	virtual void move(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) {}
};

#endif