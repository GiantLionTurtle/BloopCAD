
#include "moveCamera_action.hpp"

#include <utils/mathUtils.hpp>
#include <utils/DebugUtils.hpp>
#include <utils/preferences.hpp>
#include <document.hpp>

#include <glm/gtx/vector_angle.hpp>

moveCamera_action::moveCamera_action(Camera_ptr cam, CameraState target, long duration_ms):
	mCamera(cam),
	mTargetState({target.pos, glm::normalize(target.right), glm::normalize(target.up)}),
	mStarted(false),
	mDuration(duration_ms),
	mRotation(glm::quat()),
	mTranslation(glm::vec3()),
	mScale(glm::vec3(1.0f))
{

}

std::shared_ptr<action> moveCamera_action::create_from_facingPlane(	Geom3d::Plane_abstr* toFace, float dist_to_plane, 
																				CameraState const& camSt, Camera_ptr cam)
{
	glm::vec3 right_plane, up_plane; // The v & w vectors that will be used for the sketch's plane
	Camera::get_alignedPlaneVectors(camSt, toFace, right_plane, up_plane, true);

	CameraState targetCamState = {  toFace->origin() + glm::normalize(glm::cross(right_plane, up_plane)) * dist_to_plane, right_plane, up_plane };
	return std::shared_ptr<action>(new moveCamera_action(cam, 
	targetCamState, 
	preferences::get_instance().get_long("camtrans")));
}
std::shared_ptr<action> moveCamera_action::create_from_facingPlane(Geom3d::Plane_abstr* toFace, float dist_to_plane, Camera_ptr cam)
{
	return create_from_facingPlane(toFace, dist_to_plane, cam->state(), cam);
}

bool moveCamera_action::do_work(document* caller)
{
	if(!mStarted) {
		if(!mCamera) {
			mCamera = caller->state()->cam;
		}
		mInitState = mCamera->state();
		compute_animatables(mTargetState);
		mStarted = true;
	}

	return move_Camera();
}
bool moveCamera_action::undo_work(document* caller)
{
	if(!mStarted) {
		if(!mCamera) {
			mCamera = caller->state()->cam;
		}
		compute_animatables(mInitState);
		mStarted = true;
	}

	return move_Camera();
}

bool moveCamera_action::move_Camera()
{
	mRotation.update();
	mTranslation.update();
	mScale.update();
	mCamera->set_transformation({ mTranslation.get(), mScale.get(), mRotation.get() });

	if(mRotation.steady()) {
		mStarted = false;
		mCamera->set_orientation(glm::vec3(mFinalOrientation.x, mFinalOrientation.y, mFinalOrientation.z)); // Set it directly to prevent deadlock
		return true;
	}
	return false;
}

void moveCamera_action::compute_animatables(CameraState const& state)
{
	float angle_x, angle_y;
	CameraState currState = mCamera->state();
	
	angle_y = glm::orientedAngle(state.right, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	bool revertX = 	(Camera::flipped(state) && state.pos.y > 0. && state.pos.x > 0) ||
					(std::abs(angle_y) >= M_PI_2);
	glm::vec3 refX = glm::vec3(revertX ? -1.0f : 1.0f, 0.0f, 0.0f);

	angle_x = glm::orientedAngle(state.up, glm::vec3(0.0f, 1.0f, 0.0f), refX);

	mFinalOrientation = glm::vec3(
		mCamera->orientation().x + diff_angles(mCamera->orientation().x, angle_x), 
		mCamera->orientation().y + diff_angles(mCamera->orientation().y, angle_y),
		0.0f
	);
	
	glm::quat rotation;
	Camera::orientation_to_rotation(mFinalOrientation, rotation); // Set the quaternion to the computed orientation
	mRotation.set(mCamera->rotation());
	mRotation.set(rotation, mDuration);

	transform transf { glm::vec3(0.0f), glm::vec3(1.0f), rotation };
	mTranslation.set(glm::vec3(mCamera->translation()));
	mTranslation.set(-(state.pos-mCamera->predictedPos(transf)), mDuration);
	mScale.set(glm::vec3(1.0f));
}