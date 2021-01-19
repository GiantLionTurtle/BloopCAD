
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

float circle_abstract::dist_to_point(point_abstract_ptr pt)
{
    float dist_to_center = mCenter->dist(pt);
    if(dist_to_center < mRadius->val()) {
        return mRadius->val() - dist_to_center;
    } else {
        return dist_to_center - mRadius->val();
    }
}
float circle_abstract::dist_to_point(point_abstract const& pt)
{
    float dist_to_center = mCenter->dist(pt);
    if(dist_to_center < mRadius->val()) {
        return mRadius->val() - dist_to_center;
    } else {
        return dist_to_center - mRadius->val();
    }
}
glm::vec2 circle_abstract::closestPoint(point_abstract_ptr pt)
{
    glm::vec2 center_to_point = glm::normalize(mCenter->pos_val() - pt->pos_val());
    return glm::vec2(mCenter->pos_val()) + center_to_point * (float)mRadius->val();    
}
glm::vec2 circle_abstract::closestPoint(glm::vec2 pt)
{
    glm::vec2 center_to_point = glm::normalize(pt - glm::vec2(mCenter->pos_val()));
    return glm::vec2(mCenter->pos_val()) + center_to_point * (float)mRadius->val();    
}