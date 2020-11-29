
#include "camera.hpp"
#include <errorLogger.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/vector_angle.hpp>

int camera::numCams = 0;

camera::camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, float aspectRatio_):
	mPos(cartesianCoords),
	mTarget(target),
	mOrientation(glm::vec3(0.0f, 0.0f, 0.0f)),
	mZoom(1.0f),
	mFOV(FOV_),
	mAspectRatio(aspectRatio_),
	mTransformation{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f)},
	mID(numCams++)
{

}

glm::mat4 camera::model() const
{
	return 
	glm::translate(glm::mat4(1.0f), mTransformation.translation.get()) * 
	glm::toMat4(mTransformation.rotation.get()) *
	// glm::rotate(glm::mat4(1.0f), mTransformation.rotation.get().x, glm::vec3(1.0f, 0.0f, 0.0f)) *
	// glm::rotate(glm::mat4(1.0f), mTransformation.rotation.get().y, glm::vec3(0.0f, 1.0f, 0.0f)) *
	// glm::rotate(glm::mat4(1.0f), mTransformation.rotation.get().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
	glm::scale(glm::mat4(1.0f), mTransformation.scale.get());
}
glm::mat4 camera::model_inv() const
{
	return 
	glm::scale(glm::mat4(1.0f), 1.0f/mTransformation.scale.get()) * 
	glm::toMat4(glm::inverse(mTransformation.rotation.get())) *
	// glm::rotate(glm::mat4(1.0f), -mTransformation.rotation.get().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
	// glm::rotate(glm::mat4(1.0f), -mTransformation.rotation.get().y, glm::vec3(0.0f, 1.0f, 0.0f)) *
	// glm::rotate(glm::mat4(1.0f), -mTransformation.rotation.get().x, glm::vec3(1.0f, 0.0f, 0.0f)) *
	glm::translate(glm::mat4(1.0f), -mTransformation.translation.get());
}
glm::mat4 camera::view() const
{
	// TODO: have a competant up
	return glm::lookAt(mPos.get(), mTarget.get(), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::mat4 camera::projection() const
{
	return glm::perspective(mFOV.get(), mAspectRatio.get(), 0.1f, 100.0f);
}

glm::vec3 camera::pos() const
{
	// std::cout<<"right: "<<glm::to_string(right())<<"\n";
	// std::cout<<"pos: "<<glm::to_string(model_inv() * glm::vec4(mPos.get(), 1.0f))<<"\n";
	return model_inv() * glm::vec4(mPos.get(), 1.0f);
}
glm::vec3 camera::target() const
{
	return model_inv() * glm::vec4(mTarget.get(), 1.0f);
}

glm::vec3 camera::up() const
{
	return model_inv() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}
glm::vec3 camera::right() const
{
	return model_inv() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

glm::vec3 cart_to_sphe(glm::vec3 cart)
{
	glm::vec3 sphe;
	sphe.z = glm::length(cart);
	sphe.x = std::atan2(cart.y, cart.x);
	sphe.y = std::atan2(std::sqrt(cart.x*cart.x + cart.y*cart.y), cart.z);

	return sphe;
}

void camera::set(std::shared_ptr<camera> other)
{
	mPos.set(other->mPos.get());
	mTarget.set(other->mTarget.get());
	mOrientation.set(other->mOrientation.get());

	mZoom.set(other->mZoom.get());
	mFOV.set(other->mFOV.get());
	mAspectRatio.set(other->mAspectRatio.get());
	
	mTransformation.rotation.set(other->mTransformation.rotation.get());
	mTransformation.translation.set(other->mTransformation.translation.get());
	mTransformation.scale.set(other->mTransformation.scale.get());
}

void camera::go_to(glm::vec3 target_, glm::vec3 up_, glm::vec3 right_)
{
	go_to(target_, up_, right_, 0); // This will make the animatable set directly
}

void camera::go_to(glm::vec3 target_, glm::vec3 up_, glm::vec3 right_, unsigned int duration)
{
	glm::vec3 targetBack = glm::normalize(glm::cross(right_, up_));
	glm::vec3 natBack(0.0f, 0.0f, 1.0f);
	glm::vec3 deltaBacks = targetBack-natBack;

	float angle_x = std::asin(deltaBacks.y);
	float angle_y = -std::asin(deltaBacks.x);
	if(flipped()) {
		if(targetBack.z < 0.0f) {
			angle_x = M_PI - angle_x;
		} else {
			angle_x = M_PI * 2.0f - angle_x;
		}
		angle_y = -angle_y;
	} else {
		if(targetBack.z < 0.0f) {
			angle_y = M_PI - angle_y;
		} 
	}

	glm::vec2 angles(
		mOrientation.get().x + diff_angle(mOrientation.get().x, angle_x), 
		mOrientation.get().y + diff_angle(mOrientation.get().y, angle_y)
	);
	
	mTransformation.rotation.set(
		glm::angleAxis(angles.x, glm::vec3(1.0f, 0.0f, 0.0f)) * 
		glm::angleAxis(angles.y, glm::vec3(0.0f, 1.0f, 0.0f)), duration);
	mOrientation.set(glm::vec3(angles.x, angles.y, 0.0f));
}

void camera::update()
{
	mTarget.update();
	mZoom.update();	
	mAspectRatio.update();
	mTransformation.translation.update();

	if(mTransformation.rotation.steady()) {
		mOrientation.update();
		update_rotation();
	} else {
		mTransformation.rotation.update();
	}

	// TODO: change position, fov and scale in a way that makes sense and is coherent
	mTransformation.scale.set(glm::vec3(mZoom, mZoom, mZoom));
}

bool camera::flipped() const
{
	float mod_angle = std::fmod(mOrientation.get().x, M_PI * 2.0f);
	if(mod_angle < 0)
		mod_angle += M_PI * 2.0f;
	return (mod_angle > M_PI / 2.0f && mod_angle < M_PI * 3.0f / 2.0f);
}

void camera::update_rotation()
{
	mTransformation.rotation.set(glm::angleAxis(mOrientation.get().x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(mOrientation.get().y, glm::vec3(0.0f, 1.0f, 0.0f)));
}
