
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#ifndef CAMERASTATE_HPP_
#define CAMERASTATE_HPP_

#include <glm/glm.hpp>

/*
	@struct CameraState is a helper struct to describe a Camera at a place in space with an orientation

	@note Maybe a better name could be found, there are too many 'states' in this project
*/
struct CameraState {
	glm::vec3 pos, right, up; // Position, and vectors describing orientation
};

#endif