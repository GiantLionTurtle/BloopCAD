
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

#include "Maths_util.hpp"

#include <cmath>

float diff_angles(float current, float target)
{
	float out = target - current + M_PI; // Add PI to simplify maths
				  // The floor term brings back the answer in a reasonnable range
	return (out - (std::floor(out / (M_PI * 2.0f)) * M_PI * 2.0f)) - M_PI; // Remove PI to cancel the added PI
}