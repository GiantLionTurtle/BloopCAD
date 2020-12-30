
#include "moveCamera_action.hpp"

#include <utils/mathUtils.hpp>
#include <utils/errorLogger.hpp>

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

// void moveCamera_action::compute_animatables(camState const& state)
// {
// 	// This is still somewhat messy, it probably won't stay in this class or in this form
// 	glm::vec3 targetBack = glm::normalize(glm::cross(state.right, state.up)); // Opposite of the front that has it will have in the end
// 	glm::vec3 natBack(0.0f, 0.0f, 1.0f); // Inverse of the "real front"
// 	glm::vec3 deltaBacks = targetBack-natBack; 

// 	// Calculate the orientation based on the difference between the target and "current" backs
// 	float angle_x = std::asin(deltaBacks.y);
// 	float angle_y = -std::asin(deltaBacks.x);
// 	if(mCamera->flipped()) {
// 		if(targetBack.z < 0.0f) {
// 			angle_x = M_PI - angle_x;
// 		} else {
// 			angle_x = M_PI * 2.0f - angle_x;
// 		}
// 		angle_y = -angle_y;
// 	} else {
// 		if(targetBack.z < 0.0f) {
// 			angle_y = M_PI - angle_y;
// 		} 
// 	}

// 	mFinalOrientation = glm::vec3(
// 		mCamera->orientation().x + diff_angles(mCamera->orientation().x, angle_x), 
// 		mCamera->orientation().y + diff_angles(mCamera->orientation().y, angle_y),
// 		0.0f
// 	);
	
// 	glm::quat rotation;
// 	camera::orientation_to_rotation(mFinalOrientation, rotation); // Set the quaternion to the computed orientation
// 	mRotation.set(mCamera->transformation().rotation);
// 	mRotation.set(rotation, mDuration);
// 	// mTranslation.set(glm::vec3(mCamera->transformation().translation));
// 	// mTranslation.set(mCamera->transformation().translation-state.pos, mDuration);
// 	mScale.set(glm::vec3(1.0f));
// }

void moveCamera_action::compute_animatables(camState const& state)
{
	float angle_x, angle_y;

	angle_x = std::acos(glm::dot(state.up, glm::vec3(0.0f, 1.0f, 0.0f)));//glm::orientedAngle(state.up, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	angle_y = glm::orientedAngle(state.right, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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