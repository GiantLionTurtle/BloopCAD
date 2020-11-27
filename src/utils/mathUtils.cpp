
#include "mathUtils.hpp"

#include <cmath>

float diff_angle(float current, float target)
{
	float out = target - current + M_PI;	
	return (out - (std::floor(out / (M_PI * 2.0f)) * M_PI * 2.0f)) - M_PI;
}