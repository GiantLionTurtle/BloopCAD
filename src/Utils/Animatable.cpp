
#include "Animatable.hpp"

template<>
void Animatable<glm::quat>::update_internal(float amount)
{
	mVal = glm::mix(mInitVal, mTargetVal, amount); // You can't just multiply quaternion to lerp them
}