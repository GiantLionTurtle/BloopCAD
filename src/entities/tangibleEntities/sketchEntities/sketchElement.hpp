
#ifndef SKETCHENTITY_HPP_
#define SKETCHENTITY_HPP_

#include <entities/geometry/plane_abstract.hpp>
#include <entities/tangibleEntities/tangibleEntity.hpp>

class sketchElement;
using sketchElement_ptr = std::shared_ptr<sketchElement>;

class sketchElement {
protected:
    plane_abstract_ptr mBasePlane;
public:
    sketchElement():
        mBasePlane(nullptr)
    {}
    sketchElement(plane_abstract_ptr basePlane_):
        mBasePlane(basePlane_)
    {}
    virtual ~sketchElement() {}

    plane_abstract_ptr basePlane() const { return mBasePlane; }
    virtual void set_basePlane(plane_abstract_ptr basePlane_) { mBasePlane = basePlane_; }
};

#endif