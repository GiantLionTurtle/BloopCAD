
#ifndef MATHUTILS_HPP_
#define MATHUTILS_HPP_

/*
	@function diff_angless computes the minimum difference between two angles

	@param current : 	The first angle to evaluate
	@param target : 	The angle that should be obtained if the result was added to current

	@return : The difference between two angle [-pi, pi]
*/
float diff_angles(float current, float target);

/*
	@function map maps a number in a linear fashion between two ranges

	@param x : 			The number to map
	@param in_min : 	The minimum value of the input range
	@param in_max : 	The maximum value of the input range
	@param out_min : 	The minimum value of the output range
	@param out_max : 	The maximum value of the output range

	@return : The mapped number

	@note : This function was taken from the arduino map page and templated
*/
template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
	// https://www.arduino.cc/reference/en/language/functions/math/map/
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif