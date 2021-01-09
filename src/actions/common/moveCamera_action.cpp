
#include "moveCamera_action.hpp"

#include <utils/mathUtils.hpp>
#include <utils/errorLogger.hpp>
#include <utils/preferences.hpp>

#include <glm/gtx/vector_angle.hpp>

moveCamera_action::moveCamera_action(std::shared_ptr<camera> cam, camState target, long duration_ms):
	mCamera(cam),
	mTargetState({target.pos, glm::normalize(target.right), glm::normalize(target.up)}),
	mStarted(false),
	mDuration(duration_ms),
	mRotation(glm::quat()),
	mTranslation(glm::vec3()),
	mScale(glm::vec3(1.0f))
{

}

std::shared_ptr<action> moveCamera_action::create_from_facingPlane(	std::shared_ptr<plane_abstract> toFace, float dist_to_plane, 
																				camState const& camSt, std::shared_ptr<camera> cam)
{
	float dot_right_v 	= glm::dot(camSt.right, toFace->v()); // How similar is the camRight to the v vector?
	float dot_up_v 		= glm::dot(camSt.up, toFace->v()); // How similar is the camUp to the v vector?
	float dot_right_w 	= glm::dot(camSt.right, toFace->w());
	float dot_up_w 		= glm::dot(camSt.up, toFace->w());

	glm::vec3 right_plane, up_plane; // The v & w vectors that will be used for the sketch's plane
	glm::vec3 v_plane, w_plane;

	// Decide which of the camera vectors will be assigned to which of the plane's vectors
	if(std::abs(dot_right_w) > std::abs(dot_right_v) && (std::abs(dot_right_w) > std::abs(dot_up_w) || std::abs(dot_right_v) < std::abs(dot_up_v))) {
		right_plane =  toFace->w() * (dot_right_w < 0.0f ? -1.0f : 1.0f); // Invert it or not
		up_plane =  toFace->v() * (dot_up_v < 0.0f ? -1.0f : 1.0f); // Invert it or not
	} else {
		right_plane =  toFace->v() * (dot_right_v < 0.0f ? -1.0f : 1.0f); // Invert it or not
		up_plane =  toFace->w() * (dot_up_w < 0.0f ? -1.0f : 1.0f); // Invert it or not
	}
	camState targetCamState = {  toFace->origin() + glm::normalize(glm::cross(right_plane, up_plane)) * dist_to_plane, right_plane, up_plane };
	return std::shared_ptr<action>(new moveCamera_action(cam, 
	targetCamState, 
	preferences::get_instance().get_long("camtrans")));
}
std::shared_ptr<action> moveCamera_action::create_from_facingPlane(	std::shared_ptr<plane_abstract> toFace, float dist_to_plane, std::shared_ptr<camera> cam)
{
	return create_from_facingPlane(toFace, dist_to_plane, cam->state(), cam);
}

bool moveCamera_action::do_work()
{
	if(!mStarted) {
		mInitState = mCamera->state();
		compute_animatables(mTargetState);
		mStarted = true;
	}

	return move_camera();
}
bool moveCamera_action::undo_work()
{
	if(!mStarted) {
		compute_animatables(mInitState);
		mStarted = true;
	}

	return move_camera();
}

bool moveCamera_action::move_camera()
{
	mRotation.update();
	mTranslation.update();
	mScale.update();
	mCamera->transformation().rotation = mRotation.get();
	mCamera->transformation().translation = mTranslation.get();
	mCamera->transformation().scale = mScale.get();

	if(mRotation.steady()) {
		mStarted = false;
		mCamera->set_orientation(glm::vec3(mFinalOrientation.x, mFinalOrientation.y, mFinalOrientation.z)); // Set it directly to prevent deadlock
		return true;
	}
	return false;
}

void moveCamera_action::compute_animatables(camState const& state)
{
	float angle_x, angle_y;
	camState currState = mCamera->state();
	
	angle_y = glm::orientedAngle(state.right, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	bool revertX = 	(camera::flipped(state) && state.pos.y > 0. && state.pos.x > 0) ||
					(std::abs(angle_y) >= M_PI_2);
	glm::vec3 refX = glm::vec3(revertX ? -1.0f : 1.0f, 0.0f, 0.0f);

	angle_x = glm::orientedAngle(state.up, glm::vec3(0.0f, 1.0f, 0.0f), refX);

	mFinalOrientation = glm::vec3(
		mCamera->orientation().x + diff_angles(mCamera->orientation().x, angle_x), 
		mCamera->orientation().y + diff_angles(mCamera->orientation().y, angle_y),
		0.0f
	);
	
	glm::quat rotation;
	camera::orientation_to_rotation(mFinalOrientation, rotation); // Set the quaternion to the computed orientation
	mRotation.set(mCamera->transformation().rotation);
	mRotation.set(rotation, mDuration);

	transform transf { glm::vec3(0.0f), glm::vec3(1.0f), rotation };
	mTranslation.set(glm::vec3(mCamera->transformation().translation));
	mTranslation.set(-(state.pos-mCamera->predictedPos(transf)), mDuration);
	mScale.set(glm::vec3(1.0f));
}