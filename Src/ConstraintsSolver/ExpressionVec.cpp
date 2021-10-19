
#include "ExpressionVec.hpp"

template<>
void ExpVec2<ExpVar>::set(glm::vec2 p)
{
	x()->drag(p.x);
	y()->drag(p.y);
}