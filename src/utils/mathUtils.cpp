
#include "mathUtils.hpp"

#include <cmath>

float diff_angles(float current, float target)
{
	float out = target - current + M_PI; // Add PI to simplify maths
				  // The floor term brings back the answer in a reasonnable range
	return (out - (std::floor(out / (M_PI * 2.0f)) * M_PI * 2.0f)) - M_PI; // Remove PI to cancel the added PI
}