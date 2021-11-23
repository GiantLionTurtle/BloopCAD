
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