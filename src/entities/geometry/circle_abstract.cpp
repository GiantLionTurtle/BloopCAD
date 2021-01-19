
#include "circle_abstract.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

circle_abstract::circle_abstract(point_abstract_ptr center_, variable_ptr radius_, bool immovable):
    mCenter(center_),
    mRadius(radius_)
{
    if(immovable) {
        mCenter->set_constant();
        mRadius->set_constant();
    }
}
circle_abstract::circle_abstract(glm::vec2 center_, float radius_, bool immovable):
    mCenter(point_abstract_ptr(new point_abstract(glm::vec3(center_, 0.0f), immovable))),
    mRadius(variable_ptr(new variable(radius_)))
{
    if(immovable) {
        mRadius->set_constant();
    }
}

glm::vec2 circle_abstract::at(float angle)
{
    return glm::vec2(mCenter->pos_val()) + glm::vec2(std::cos(angle), std::sin(angle)) * (float)mRadius->val();
}