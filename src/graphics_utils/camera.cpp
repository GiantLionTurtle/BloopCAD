
#include "camera.hpp"
#include <errorLogger.hpp>

int camera::numCams = 0;

camera::camera(glm::vec3 cartesianCoords, glm::vec3 target, float zoom_, float aspectRatio_):
	mPos_spherical(0.0f, 0.0f, 0.0f),
	mPos_cartesian(cartesianCoords),
	mTarget(target),
	mZoom(zoom_),
	mAspectRatio(aspectRatio_),
	mIs_flipped(false),
	mID(numCams++),
	mFinishedMove(true)
{
	updatePos_spherical();
	updateUp();
}
camera::camera(glm::vec3 target, float zoom_, float aspectRatio_):
	mPos_spherical(0.0f, 0.0f, 0.0f),
	mPos_cartesian(0.0f, 0.0f, 0.0f),
	mTarget(target),
	mZoom(zoom_),
	mAspectRatio(aspectRatio_),
	mIs_flipped(false),
	mID(numCams++),
	mFinishedMove(true)
{
	updateUp();
}

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
	return glm::lookAt(mPos_cartesian, mTarget, glm::vec3(0.0f, (mIs_flipped) ? -1.0f : 1.0f, 0.0f));
}

glm::mat4 camera::projection() const
{
	return glm::perspective(glm::radians(mZoom), mAspectRatio, 0.1f, 100.0f);
}

void camera::move_spherical(glm::vec3 delta)
{
	mPos_spherical += delta;
	updatePos_cartesian();
	updateUp();
}
void camera::set_spherical(glm::vec3 spherical)
{
	mPos_spherical = spherical;
	updatePos_cartesian();
	updateUp();
}
void camera::move_cartesian(glm::vec3 delta)
{
	mPos_cartesian += delta;
	updatePos_spherical();
	updateUp();
}
void camera::set_cartesian(glm::vec3 cartesian)
{
	mPos_cartesian = cartesian;
	updatePos_spherical();
	updateUp();
}

void camera::move_target(glm::vec3 delta)
{
	mTarget += delta;
}
void camera::set_target(glm::vec3 target)
{
	mTarget = target;
}

void camera::set_targetState(camState state, unsigned int moveDuration)
{	
	mMoveEnd = std::chrono::steady_clock::now();
	mMoveEnd += std::chrono::milliseconds(moveDuration);
	mMoveDuration = moveDuration;
	mMoveInitState = { state.cartesian, (state.cartesian ? mPos_cartesian : mPos_spherical), mTarget };
	mMoveFinalState = state;
	mFinishedMove = false;
}

void camera::update_move()
{
	if(mFinishedMove)
		return;
	float timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(mMoveEnd-std::chrono::steady_clock::now()).count();
	if(timeLeft > 0) {
		float amount = timeLeft / mMoveDuration;
		if(mMoveInitState.cartesian) {
			set_cartesian(mMoveFinalState.Pos + (mMoveFinalState.Pos - mMoveInitState.Pos) * amount);
		} else {
			set_spherical(mMoveFinalState.Pos + (mMoveFinalState.Pos - mMoveInitState.Pos) * amount);
		}
		mTarget += mMoveFinalState.Target + (mMoveFinalState.Target - mMoveInitState.Target) * amount;
	} else {
		if(mMoveFinalState.cartesian) {
			set_cartesian(mMoveFinalState.Pos);
		} else {
			set_spherical(mMoveFinalState.Pos);
		}
		mTarget = mMoveFinalState.Target;
		mFinishedMove = true;
	}
}

void camera::updatePos_cartesian()
{
	mPos_cartesian = glm::vec3(
		mPos_spherical.x * std::sin(mPos_spherical.z) * std::cos(mPos_spherical.y),
		mPos_spherical.x * std::cos(mPos_spherical.z),
		mPos_spherical.x * std::sin(mPos_spherical.z) * std::sin(mPos_spherical.y)
	);
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
}

void camera::updateUp()
{
	glm::vec3 dir_inv = glm::normalize(mPos_cartesian - mTarget);
	float modz = std::fmod(mPos_spherical.z, 6.283);
	if(modz < 0.0f)
		modz = 6.283f + modz;
	mIs_flipped = (modz > 3.1415);
	glm::vec3 up_tmp = glm::vec3(0.0f, (mIs_flipped) ? -1.0f : 1.0f, 0.0f); // global up
	mRight = glm::normalize(glm::cross(up_tmp, dir_inv));
	mUp = glm::cross(dir_inv, mRight);
}