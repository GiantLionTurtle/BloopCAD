
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <iostream>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>


// https://en.wikipedia.org/wiki/Spherical_coordinate_system
// https://math.libretexts.org/Bookshelves/Calculus/Book%3A_Calculus_(OpenStax)/12%3A_Vectors_in_Space/12.7%3A_Cylindrical_and_Spherical_Coordinates
class camera {
protected:
	static int numCams;
	int mID;
	glm::vec3 mPos_spherical, mPos_cartesian, mTarget;
	glm::mat4 mProjection;
	float mZoom, mAspectRatio;
	glm::vec3 mUp, mRight;
	glm::vec3 mFront;
	bool mIs_flipped;
public:
	camera(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_);
	camera(glm::vec3 target, float zoom, float aspectRatio_);

	static camera from_cartesian(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_);
	static camera from_spherical(glm::vec3 sphericalCoords, glm::vec3 target, float zoom_, float aspectRatio_);

	static std::shared_ptr<camera> from_cartesian_ptr(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_);
	static std::shared_ptr<camera> from_spherical_ptr(glm::vec3 sphericalCoords, glm::vec3 target, float zoom_, float aspectRatio_);

	glm::mat4 view() const;
	glm::mat4 projection() const;

	glm::vec3 pos_cartesian() const { return mPos_cartesian; }
	glm::vec3 pos_spherical() const { return mPos_spherical; }

	void move_spherical(glm::vec3 delta);
	void set_spherical(glm::vec3 sperical);
	void move_cartesian(glm::vec3 delta);
	void set_cartesian(glm::vec3 cartesian);

	void move_target(glm::vec3 delta);
	void set_target(glm::vec3 target);
	void move_front(glm::vec3 delta);

	float zoom() const { return mZoom; }
	void change_zoom(float delta) { mZoom += delta; };
	void set_zoom(float zoom_) { mZoom = zoom_; }

	float aspectRatio() const { return mAspectRatio; }
	void set_aspectRatio(float aspectRatio_) { mAspectRatio = aspectRatio_; }

	bool flipped() const { return mIs_flipped; }

	glm::vec3 up() const { return mUp; }
	glm::vec3 right() const { return mRight; }

	void updatePos_cartesian();
	void updatePos_spherical();
	void updateUp();

	int id() const { return mID; }
};

#endif
