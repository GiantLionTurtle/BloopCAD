
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "sketchElement.hpp"
#include <entities/tangibleEntities/point.hpp>

class sketchPoint;
using sketchPoint_ptr = std::shared_ptr<sketchPoint>;

class sketchPoint : public sketchElement, public point {
public:
	sketchPoint(glm::vec2 pos_2d, plane_abstract_ptr basePlane_);
	virtual ~sketchPoint() {}

	virtual glm::vec3 pos_vec() const;

	virtual void set_pos(glm::vec2 pos);
	virtual void set_pos(glm::vec3 pos);
	virtual void set_pos(point_abstract const& other);

    virtual void set_basePlane(plane_abstract_ptr basePlane_) { mBasePlane = basePlane_; mRequire_VBUpdate = true; }
};

#endif 