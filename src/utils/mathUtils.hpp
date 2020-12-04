
#ifndef MATHUTILS_HPP_
#define MATHUTILS_HPP_

float diff_angle(float current, float target);

template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
	// https://www.arduino.cc/reference/en/language/functions/math/map/
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif