
#ifndef SKETCHENTITY_HPP_
#define SKETCHENTITY_HPP_

#include <geometry/geometry_3d/plane_abstr.hpp>
#include <entities/tangibleEntities/tangibleEntity.hpp>

#include <memory>

class sketchEntity;
using sketchEntity_ptr = std::shared_ptr<sketchEntity>;

class entityPosSnapshot_abstract;
using entityPosSnapshot_ptr = std::shared_ptr<entityPosSnapshot_abstract>;

class sketchEntity : public tangibleEntity {
public:
	enum types { POINT, LINE, CIRCLE, SPRITE, CONSTRAINT };
protected:
	geom_3d::plane_abstr_ptr mBasePlane;
	int mType;
	bool mDragged;
public:
	sketchEntity(geom_3d::plane_abstr_ptr basePlane_, int type_):
		mType(type_),
		mBasePlane(basePlane_),
		mDragged(false)
	{}
	virtual ~sketchEntity() {}

	virtual void for_each(std::function<void(sketchEntity_ptr geom)> func) {}

	virtual void move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move) {}
	bool dragged() const { return mDragged; }
	void set_dragged(bool drag) { mDragged = drag; set_dragged_impl(drag); } //set_tmpConstant(drag); }

	virtual bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor) { return false; }
	virtual bool in_selection_range(glm::vec2 a, glm::vec2 b, bool contained) { return false; }	

	virtual void set_constant() {};
	virtual void set_tmpConstant(bool const_) {}

	geom_3d::plane_abstr_ptr basePlane() const { return mBasePlane; }
	virtual void set_basePlane(geom_3d::plane_abstr_ptr basePlane_) { mBasePlane = basePlane_; set_require_VBUpdate(); }

	int type() { return mType; }

	virtual void set_exists_vars(bool ex) {}

	virtual entityPosSnapshot_ptr posSnapshot() { return nullptr; }
protected:
	virtual void exists_impl(bool ex) { notify_parent(ex ? RESURRECTED : DELETED); set_exists_vars(ex); }

	virtual void set_dragged_impl(bool drag) {}
};

#endif