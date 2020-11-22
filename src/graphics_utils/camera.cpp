
#include "camera.hpp"
#include <errorLogger.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

int camera::numCams = 0;

camera::camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, float aspectRatio_):
	mPos(cartesianCoords),
	mTarget(target),
	mSphere(glm::vec2(0.0f, 0.0f)),
	mZoom(1.0f),
	mFOV(FOV_),
	mAspectRatio(aspectRatio_),
	mTransformation{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 
	// glm::vec3(0.0f, 0.0f, 0.0f)},
	glm::quat(1.0f, 0.0f, 0.0f, 0.0f)},
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

void camera::set(std::shared_ptr<camera> other)
{
	mPos.set(other->mPos.get());
	mTarget.set(other->mTarget.get());

	mZoom.set(other->mZoom.get());
	mFOV.set(other->mFOV.get());
	mAspectRatio.set(other->mAspectRatio.get());
	
	mTransformation.rotation.set(other->mTransformation.rotation.get());
	mTransformation.translation.set(other->mTransformation.translation.get());
	mTransformation.scale.set(other->mTransformation.scale.get());
}

void camera::go_to(glm::vec3 lookat_, glm::vec3 up_, glm::vec3 right_)
{

}
void camera::go_to(glm::vec3 target_, glm::vec3 up_, glm::vec3 right_, unsigned int duration)
{

}

void camera::update()
{
	mTarget.update();
	mZoom.update();	
	mAspectRatio.update();
	mTransformation.translation.update();
	mTransformation.rotation.update();

	// TODO: change position, fov and scale in a way that makes sense and is coherent
	mTransformation.scale.set(glm::vec3(mZoom, mZoom, mZoom));

	mTransformation.rotation.set(glm::angleAxis(mSphere.get().x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(mSphere.get().y, glm::vec3(0.0f, 1.0f, 0.0f)));
}