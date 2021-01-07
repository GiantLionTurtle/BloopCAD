
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "sketchEntity.hpp"
#include <entities/tangibleEntities/point.hpp>

class sketchPoint : public sketchEntity, public point {
public:
	sketchPoint(std::shared_ptr<plane_abstract> basePlane_, glm::vec2 pos_2d);
	virtual ~sketchPoint() {}

	virtual void set_pos(glm::vec2 pos);
	virtual void set_pos(glm::vec3 pos);
	virtual void set_pos(point_abstract const& other);

    virtual void set_basePlane(std::shared_ptr<plane_abstract> basePlane_) { mBasePlane = basePlane_; mRequire_VBUpdate = true; }
};

#endif 