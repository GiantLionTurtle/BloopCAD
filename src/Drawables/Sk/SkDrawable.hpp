
#ifndef SKETCHDRAWABLE_HPP_
#define SKETCHDRAWABLE_HPP_

#include <geometry/geometry_3d/plane_abstr.hpp>
#include <Drawables/Drawable.hpp>
#include <graphics_utils/Camera.hpp>

class SkDrawable;

struct DraggableSelectionPoint {
	DraggableSelectionPoint():
		ent(nullptr)
	{}
	DraggableSelectionPoint(SkDrawable* dr):
		ent(dr)
	{}

	SkDrawable* ent;
	// expression point of some sort
};

class SkDrawable : virtual public Drawable {
protected:
	Geom3d::plane_abstr* mBasePlane; // Should it be the parent sketch directly??
public:
	SkDrawable(Geom3d::plane_abstr* pl):
		mBasePlane(pl)
	{
		mType |= Drawable_types::SKDRAWABLE;
	}

	Geom3d::plane_abstr* basePlane() { return mBasePlane; }
	void set_basePlane(Geom3d::plane_abstr* pl) { mBasePlane = pl; set_need_update(); }

	virtual SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual void move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) { if(selected()) move(start, end, pix_mov); }
	virtual void move(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) {}
};

#endif