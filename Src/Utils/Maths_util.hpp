
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MATHUTILS_HPP_
#define MATHUTILS_HPP_

#include <glm/glm.hpp>

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

// Maths (and corresponding unit test) from 
// https://gamedev.stackexchange.com/questions/70075/how-can-i-find-the-perpendicular-to-a-2d-vector
template<typename T>
glm::vec<2, T, glm::defaultp> perpendicular_cw(glm::vec<2, T, glm::defaultp> in)
{
	return glm::vec<2, T, glm::defaultp>(in.y, -in.x);
}
template<typename T>
glm::vec<2, T, glm::defaultp> perpendicular_ccw(glm::vec<2, T, glm::defaultp> in)
{
	return glm::vec<2, T, glm::defaultp>(-in.y, in.x);
}

#endif