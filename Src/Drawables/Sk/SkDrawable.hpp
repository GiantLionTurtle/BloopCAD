
#ifndef SKETCHDRAWABLE_HPP_
#define SKETCHDRAWABLE_HPP_

#include <Drawables/Base/Drawable.hpp>
#include <Geometry/3d/Plane_abstr.hpp>
#include <ConstraintsSolver/Expression.hpp>

class SkIntDrawable;
class SkGeometry;

struct SkGeomDragPoint {
	SkGeomDragPoint()
		: geom(nullptr)
	{

	}
	SkGeomDragPoint(SkGeometry* dr)
		: geom(dr)
	{
		
	}

	operator bool() { return geom; }

	SkGeometry* geom;
};

class SkDrawable : virtual public Drawable {
public:
	enum SkNotifiers { SET = 5 };
protected:
	Geom3d::Plane_abstr* mBasePlane; // Should it be the parent sketch directly??
public:
	SkDrawable(Geom3d::Plane_abstr* pl)
		: mBasePlane(pl)
	{
		mType |= Drawable_types::SKDRAWABLE;
	}

	Geom3d::Plane_abstr* basePlane() { return mBasePlane; }
	void set_basePlane(Geom3d::Plane_abstr* pl) { mBasePlane = pl; update(); }
};

// interactive drawables
class SkIntDrawable : public SkDrawable {
public:
	SkIntDrawable(Geom3d::Plane_abstr* pl)
		: SkDrawable(pl)
	{
		
	}
	virtual bool closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual void move_selected(glm::vec2 delta) { if(selected()) move(delta); }
	virtual void move(glm::vec2 delta) {}
	virtual void release() {}
};

#endif