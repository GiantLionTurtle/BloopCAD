
#include "ExpressionVec.hpp"

template<>
void ExpVec2<ExpVar>::set(glm::vec2 p)
{
    x()->set(p.x);
    y()->set(p.y);
}