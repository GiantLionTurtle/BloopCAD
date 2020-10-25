
#include "camera.hpp"

camera::camera(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_):
	mPos_spherical(0.0f, 0.0f, 0.0f),
	mPos_cartesian(cartesianCoords),
	mTarget(target),
	mZoom(zoom_),
	mAspectRatio(aspectRatio_)
{
	updatePos_spherical();
}
camera::camera(glm::vec3 target, float zoom_, float aspectRatio_):
	mPos_spherical(0.0f, 0.0f, 0.0f),
	mPos_cartesian(0.0f, 0.0f, 0.0f),
	mTarget(target),
	mZoom(zoom_),
	mAspectRatio(aspectRatio_)
{}

camera camera::from_cartesian(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_)
{
	return camera(cartesianCoords, target, zoom_, aspectRatio_);
}
camera camera::from_spherical(glm::vec3 sphericalCoords, glm::vec3 target, float zoom_, float aspectRatio_)
{
	camera out(target, zoom_, aspectRatio_);
	out.set_spherical(sphericalCoords);
	return out;
}

std::shared_ptr<camera> camera::from_cartesian_ptr(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_)
{
	return std::shared_ptr<camera>(new camera(cartesianCoords, target, zoom_, aspectRatio_));
}
std::shared_ptr<camera> camera::from_spherical_ptr(glm::vec3 sphericalCoords, glm::vec3 target, float zoom_, float aspectRatio_)
{
	std::shared_ptr<camera> out(new camera(target, zoom_, aspectRatio_));
	out->set_spherical(sphericalCoords);
	return out;
}

glm::mat4 camera::view() const
{
	// TODO: have a competant up
	return glm::lookAt(mPos_cartesian, mTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 camera::projection() const
{
	return glm::perspective(glm::radians(mZoom), mAspectRatio, 1.0f, 4.0f);
}

void camera::move_spherical(glm::vec3 delta)
{
	mPos_spherical += delta;
	updatePos_cartesian();
}
void camera::set_spherical(glm::vec3 spherical)
{
	mPos_spherical = spherical;
	updatePos_cartesian();
}
void camera::move_cartesian(glm::vec3 delta)
{
	mPos_cartesian += delta;
	updatePos_spherical();
}
void camera::set_cartesian(glm::vec3 cartesian)
{
	mPos_cartesian = cartesian;
	updatePos_spherical();
}

void camera::updatePos_cartesian()
{
	mPos_cartesian = glm::vec3(
		mPos_spherical.x * std::sin(mPos_spherical.z) * std::cos(mPos_spherical.y),
		mPos_spherical.x * std::cos(mPos_spherical.z),
		mPos_spherical.x * std::sin(mPos_spherical.z) * std::sin(mPos_spherical.y)
	);
	// std::cout<<"New pos cart: "<< glm::to_string(mPos_cartesian)<<"\n";
}
void camera::updatePos_spherical()
{
	mPos_spherical = glm::vec3(
		glm::length(mPos_cartesian),
		std::atan(mPos_cartesian.y/mPos_cartesian.x),
		std::atan(std::sqrt(std::pow(mPos_cartesian.x, 2.0f) + std::pow(mPos_cartesian.y, 2.0f)) / mPos_cartesian.z)
	);
	if(mPos_cartesian.x == 0) {
		mPos_spherical.y = 3.1415 / 2.0;
	}
	if(mPos_cartesian.y == 0) {
		mPos_spherical.z = 3.1415 / 2.0;
	}
	// std::cout<<"New pos sphe: "<<glm::to_string(mPos_spherical)<<"\n";
}