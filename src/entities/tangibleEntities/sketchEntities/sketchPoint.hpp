
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "sketchElement.hpp"
#include <entities/tangibleEntities/point.hpp>

class sketchPoint;
using sketchPoint_ptr = std::shared_ptr<sketchPoint>;

class sketchPoint : public sketchElement, public point {
public:
	sketchPoint(glm::vec2 pos_2d, plane_abstract_ptr basePlane_, bool immovable = false);
	virtual ~sketchPoint() {}

	virtual glm::vec3 pos_val() const;

	virtual void set(glm::vec2 pos);
	virtual void set(glm::vec3 pos);
	virtual void set(point_abstract const& other);

    virtual void set_basePlane(plane_abstract_ptr basePlane_) { mBasePlane = basePlane_; mRequire_VBUpdate = true; }

	virtual std::vector<variable_ptr> variables();

	virtual subEquationsSystem coincidence();
	virtual std::vector<subEquationsSystem> verticality();
	virtual std::vector<subEquationsSystem> horizontality();
};

#endif 