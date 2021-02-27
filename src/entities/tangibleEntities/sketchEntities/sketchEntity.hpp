
#ifndef SKETCHENTITY_HPP_
#define SKETCHENTITY_HPP_

#include <geometry/geometry_3d/plane_abstr.hpp>
#include <entities/tangibleEntities/tangibleEntity.hpp>

#include <memory>

class sketch;
using sketch_ptr = std::shared_ptr<sketch>;

class sketchEntity;
using sketchEntity_ptr = std::shared_ptr<sketchEntity>;

class sketchEntity : public tangibleEntity {
public:
	enum types { POINT, LINE, CIRCLE };
protected:
	geom_3d::plane_abstr_ptr mBasePlane;
	int mType;
public:
	sketchEntity(geom_3d::plane_abstr_ptr basePlane_, int type):
		mType(type),
		mBasePlane(basePlane_)
	{}
	virtual ~sketchEntity() {}

	virtual void move(glm::vec2 from, glm::vec2 to) {}

	virtual bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor) { return false; }

	geom_3d::plane_abstr_ptr basePlane() const { return mBasePlane; }
	virtual void set_basePlane(geom_3d::plane_abstr_ptr basePlane_) { mBasePlane = basePlane_; set_require_VBUpdate(); }

	int type() { return mType; }
};

#endif