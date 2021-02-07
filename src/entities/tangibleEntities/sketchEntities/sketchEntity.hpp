
#ifndef SKETCHENTITY_HPP_
#define SKETCHENTITY_HPP_

#include <geometry/geometry_3d/plane_abstr.hpp>
#include <entities/tangibleEntities/tangibleEntity.hpp>
#include <constraintsSolver/DR_planner.hpp>

#include <memory>

class sketchEntity;
using sketchEntity_ptr = std::shared_ptr<sketchEntity>;

class sketchEntity : public tangibleEntity, public cluster {
protected:
    geom_3d::plane_abstr_ptr mBasePlane;
public:
    sketchEntity(geom_3d::plane_abstr_ptr basePlane_, int dof):
        cluster(dof),
        mBasePlane(basePlane_)
    {}
    virtual ~sketchEntity() {}

    virtual void move(glm::vec2 from, glm::vec2 to) {}

    geom_3d::plane_abstr_ptr basePlane() const { return mBasePlane; }
    virtual void set_basePlane(geom_3d::plane_abstr_ptr basePlane_) { mBasePlane = basePlane_; set_require_VBUpdate(); }
};

#endif