
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <animatable.hpp>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <memory>

struct transform {
	animatable<glm::vec3> translation;
	animatable<glm::vec3> scale;
	animatable<glm::quat> rotation;
	// animatable<glm::vec3> rotation;
};

// https://en.wikipedia.org/wiki/Spherical_coordinate_system
// https://math.libretexts.org/Bookshelves/Calculus/Book%3A_Calculus_(OpenStax)/12%3A_Vectors_in_Space/12.7%3A_Cylindrical_and_Spherical_Coordinates
class camera {
protected:
	static int numCams;
	int mID;
	animatable<glm::vec3> mPos, mTarget;
	animatable<glm::vec3> mOrientation;

	animatable<float> mZoom;
	animatable<float> mFOV;
	animatable<float> mAspectRatio;
	transform mTransformation;
public:
	camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, float aspectRatio_);

	glm::mat4 model() const;
	glm::mat4 model_inv() const;
	glm::mat4 view() const;
	glm::mat4 projection() const;

	glm::vec3 pos() const;
	glm::vec3 target() const;

	glm::vec3 up() const;
	glm::vec3 right() const;
	
	// animatable<glm::vec2>& sphere() { return mSphere; }
	animatable<glm::vec3>& orientation() { return mOrientation; }

	// animatable<float>& FOV() { return mFOV; }
	animatable<float>& zoom() { return mZoom; };
	animatable<float>& aspectRatio() { return mAspectRatio; }
	transform& transformation() { return mTransformation; }

	void set(std::shared_ptr<camera> other);

	void go_to(glm::vec3 target_, glm::vec3 up_, glm::vec3 right_);
	void go_to(glm::vec3 target_, glm::vec3 up_, glm::vec3 right_, unsigned int duration);
	void go_to(glm::vec2 sp, unsigned int duration);

	void update();

	int id() const { return mID; }

	bool flipped() const;
private:
	void update_rotation();
	void update_orientation();
};

#endif
