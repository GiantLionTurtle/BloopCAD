
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
	geom_3d::plane_abstr_ptr mBasePlane; // Should it be the parent sketch directly??
public:
	SkDrawable(geom_3d::plane_abstr_ptr pl):
		mBasePlane(pl)
	{
		mType |= Drawable_types::SKDRAWABLE;
	}

	geom_3d::plane_abstr_ptr basePlane() { return mBasePlane; }
	void set_basePlane(geom_3d::plane_abstr_ptr pl) { mBasePlane = pl; set_need_update(); }

	virtual SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual void move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) { if(selected()) move(start, end, pix_mov); }
	virtual void move(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov) {}
};

#endif