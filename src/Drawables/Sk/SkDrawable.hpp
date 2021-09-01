
#ifndef SKETCHDRAWABLE_HPP_
#define SKETCHDRAWABLE_HPP_

#include <Geom/Geom3d/Plane_abstr.hpp>
#include <Drawables/Tangible.hpp>
#include <ConstraintsSolver/Expression.hpp>
#include <ConstraintsSolver/ExpressionVec.hpp>

class SkIntDrawable;

struct SkExpSelPoint {
	SkExpSelPoint():
		ent(nullptr)
	{

	}
	SkExpSelPoint(SkIntDrawable* dr, ExpVec2<Expression_abstr> p):
		ent(dr),
		pt(p)
	{
		
	}

	operator bool() { return ent; }

	SkIntDrawable* ent;
	ExpVec2<Expression_abstr> pt;
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
};

// interactive drawables
class SkIntDrawable : public SkDrawable {
public:
	SkIntDrawable(Geom3d::Plane_abstr* pl):
		SkDrawable(pl)
	{
		
	}
	virtual SelPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual SkExpSelPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual void move_selected(glm::vec2 delta) { if(selected()) move(delta); }
	virtual void move(glm::vec2 delta) {}
};

#endif