
#ifndef SKETCHENTITY_HPP_
#define SKETCHENTITY_HPP_

#include <entities/geometry/plane_abstract.hpp>
#include <entities/tangibleEntities/tangibleEntity.hpp>

class sketchEntity {
protected:
    std::shared_ptr<plane_abstract> mBasePlane;
public:
    sketchEntity():
        mBasePlane(nullptr)
    {}
    sketchEntity(std::shared_ptr<plane_abstract> basePlane_):
        mBasePlane(basePlane_)
    {}
    virtual ~sketchEntity() {}

    std::shared_ptr<plane_abstract> basePlane() const { return mBasePlane; }
    virtual void set_basePlane(std::shared_ptr<plane_abstract> basePlane_) { mBasePlane = basePlane_; }
};

#endif