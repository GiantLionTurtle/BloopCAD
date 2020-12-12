
#include "camera.hpp"
#include <utils/errorLogger.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/vector_angle.hpp>

camera::camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, float aspectRatio_):
	mPos(cartesianCoords),
	mTarget(target),
	mOrientation(glm::vec3(0.0f, 0.0f, 0.0f)),
	mZoom(1.0f),
	mFOV(FOV_),
	mAspectRatio(aspectRatio_),
	mTransformation{glm::vec3(0.0f, 0.0f, 0.0f)/*No translation*/, 
		glm::vec3(1.0f, 1.0f, 1.0f)/*Scale of 100%*/, 
		glm::quat(1.0f, 0.0f, 0.0f, 0.0f)/*No rotation with unit quaternion*/}
{

}

glm::mat4 camera::model() const
{
	// The order is important!
	return 
	glm::translate(glm::mat4(1.0f), mTransformation.translation.get()) * 
	glm::toMat4(mTransformation.rotation.get()) *
	glm::scale(glm::mat4(1.0f), mTransformation.scale.get());
}
glm::mat4 camera::model_inv() const
{
	// The order is important! - it is the inverse of the model's order
	return 
	glm::scale(glm::mat4(1.0f), 1.0f/mTransformation.scale.get()) * 
	glm::toMat4(glm::inverse(mTransformation.rotation.get())) *
	glm::translate(glm::mat4(1.0f), -mTransformation.translation.get());
}
glm::mat4 camera::view() const
{
	// Here the "real" pos and target are used, not the computed ones
	return glm::lookAt(mPos.get(), mTarget.get(), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::mat4 camera::projection() const
{
	// 0.1f for the close plane and 100.0f for the far plane are arbitrary, they are subject to change
	return glm::perspective(mFOV.get(), mAspectRatio.get(), 0.1f, 100.0f);
}
glm::mat4 camera::mvp() const
{
	return projection() * view() * model(); // The order of multiplication makes this slightly awkward but model is first
}

glm::vec3 camera::pos() const
{
	return model_inv() * glm::vec4(mPos.get(), 1.0f); // Apply the inverse transform to the "real" position
}
glm::vec3 camera::target() const
{
	return model_inv() * glm::vec4(mTarget.get(), 1.0f); // Apply the inverse transform to the "real" target
}

glm::vec3 camera::up() const
{
	return model_inv() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);  // Apply the inverse transform to the "real" up vector
}
glm::vec3 camera::right() const
{
	return model_inv() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);  // Apply the inverse transform to the "real" right vector
}
glm::vec3 camera::front() const
{
	return model_inv() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);  // Apply the inverse transform to the "real" front vector
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
	// This is still somewhat messy, it probably won't stay in this class or in this form

	glm::vec3 targetBack = glm::normalize(glm::cross(right_, up_)); // Opposite of the front that has it will have in the end
	glm::vec3 natBack(0.0f, 0.0f, 1.0f); // Inverse of the "real front"
	glm::vec3 deltaBacks = targetBack-natBack; 

	// Calculate the orientation based on the difference between the target and "current" backs
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

	glm::vec3 angles(
		mOrientation.get().x + diff_angles(mOrientation.get().x, angle_x), 
		mOrientation.get().y + diff_angles(mOrientation.get().y, angle_y),
		0.0f
	);
	
	update_rotation(angles, duration); // Set the quaternion to the computed orientation
	mOrientation.set(glm::vec3(angles.x, angles.y, 0.0f)); // Set it directly to prevent deadlock
}

void camera::update()
{
	mTarget.update();
	mZoom.update();	
	mAspectRatio.update();
	mTransformation.translation.update();

	if(mTransformation.rotation.steady()) { // Do update the quaternion rotation if it is not in the middle of an interpolation
		mOrientation.update();
		update_rotation(mOrientation.get());
	} else { // The camera is performing an interpolation between rotations, do not override the quaternion
		mTransformation.rotation.update();
	}

	// TODO: change position, fov and scale in a way that makes sense and is coherent
	mTransformation.scale.set(glm::vec3(mZoom, mZoom, mZoom));
}

bool camera::flipped() const
{
	// The camera is flipped it's rotation around the x axis is between 90 and 270 degrees
	float mod_angle = std::fmod(mOrientation.get().x, M_PI * 2.0f);
	if(mod_angle < 0)
		mod_angle += M_PI * 2.0f;
	return (mod_angle > M_PI / 2.0f && mod_angle < M_PI * 3.0f / 2.0f);
}

void camera::update_rotation(glm::vec3 const& orientation, unsigned int duration_ms)
{
	// Create the quaternion 
	mTransformation.rotation.set(	glm::angleAxis(orientation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * 
									glm::angleAxis(orientation.y, glm::vec3(0.0f, 1.0f, 0.0f)), duration_ms);
}
