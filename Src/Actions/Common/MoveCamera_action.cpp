
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "MoveCamera_action.hpp"

#include <Utils/Maths_util.hpp>
#include <Utils/Debug_util.hpp>
#include <Utils/Preferences.hpp>
#include <Workspaces/Document.hpp>

#include <glm/gtx/vector_angle.hpp>

MoveCamera_action::MoveCamera_action(Camera* cam, CameraState target, long duration_ms)
	: mCamera(cam)
	, mTargetState({target.pos, glm::normalize(target.right), glm::normalize(target.up)}) // Sanitize input, maybe not necessary 
	, mStarted(false)
	, mDuration(duration_ms)

	// Alright defaults but should be overwriten before anything happens anyway..
	, mRotation(glm::quat())
	, mTranslation(glm::vec3())
	, mScale(glm::vec3(1.0f))
{

}

Action_ptr MoveCamera_action::create_from_facingPlane(	Geom3d::Plane_abstr* toFace, float dist_to_plane, 
																				CameraState const& camSt, Camera* cam)
{
	// Get the plane vectors that are closest to the camera's up&right vectors (inverting allowed)
	glm::vec3 right_plane, up_plane; 
	Camera::get_alignedPlaneVectors(camSt, toFace, right_plane, up_plane, true); 

	// Create target state from new information
	CameraState targetCamState = {  toFace->origin() + glm::normalize(glm::cross(right_plane, up_plane)) * dist_to_plane, right_plane, up_plane };
	
	return Action_ptr(new MoveCamera_action(cam, 
	targetCamState, 
	Preferences::get_instance().get_long("camtrans"))); // Weak way to set the duration but anyway..
}
Action_ptr MoveCamera_action::create_from_facingPlane(Geom3d::Plane_abstr* toFace, float dist_to_plane, Camera* cam)
{
	return create_from_facingPlane(toFace, dist_to_plane, cam->state(), cam);
}

bool MoveCamera_action::do_work(Document* caller)
{
	if(!mStarted) {
		if(!mCamera) {
			mCamera = caller->state()->cam;
		}
		mInitState = mCamera->state();
		compute_animatables(mTargetState);
		mStarted = true;
	}

	return move_camera();
}
bool MoveCamera_action::undo_work(Document* caller)
{
	if(!mStarted) {
		DEBUG_ASSERT(mCamera, "Null camera.");
		compute_animatables(mInitState);
		mStarted = true;
	}

	return move_camera();
}

bool MoveCamera_action::move_camera()
{
	// Update all the animatables
	mRotation.update();
	mTranslation.update();
	mScale.update();
	mCamera->set_transformation({ mTranslation.get(), mScale.get(), mRotation.get() });

	// If the rotation is no longer changing (it is the last iteration, 
	// set the camera orientation to it's exact value to avoir floating point errors)
	if(mRotation.steady()) {
		mStarted = false;
		mCamera->set_orientation(glm::vec3(mFinalOrientation.x, mFinalOrientation.y, mFinalOrientation.z)); // Set it directly to prevent deadlock
		return true;
	}
	return false;
}

void MoveCamera_action::compute_animatables(CameraState const& state)
{
	float angle_up, angle_right; // Spherical coordinate (ish) angles
	CameraState currState = mCamera->state(); // Reference state
	
	// Angle between the right of the camera and the world's x-axis, with the world's y axis as reference
	angle_right = glm::orientedAngle(state.right, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Check if the camera is upside down then compute the angle between the up axis of the camera and the 
	// y axis of the world with a fliped-or-not x axis from the world as reference
	bool revertX = 	(Camera::flipped(state) && state.pos.y > 0. && state.pos.x > 0) ||
					(std::abs(angle_right) >= M_PI_2);
	glm::vec3 refX = glm::vec3(revertX ? -1.0f : 1.0f, 0.0f, 0.0f);
	angle_up = glm::orientedAngle(state.up, glm::vec3(0.0f, 1.0f, 0.0f), refX);

	// The orientation that will be set on the last iteration of a move
	mFinalOrientation = glm::vec3(
		mCamera->orientation().x + diff_angles(mCamera->orientation().x, angle_up), 
		mCamera->orientation().y + diff_angles(mCamera->orientation().y, angle_right),
		0.0f
	);

	// Subtle difference between rotation and orientation
	// rotation is the quaternion representing the orientation of the
	// camera in 3d space, and the orientation is specificaly for this
	// orbit camera, it has two angles to describe it's position
	// and another to describe it's tilt (basically a twist on the screen)	
	glm::quat rotation;
	Camera::orientation_to_rotation(mFinalOrientation, rotation); // Set the quaternion to the computed orientation
	mRotation.set(mCamera->rotation()); // Initial state
	mRotation.set(rotation, mDuration); // Final state with duration

	transform transf { glm::vec3(0.0f), glm::vec3(1.0f), rotation };
	mTranslation.set(glm::vec3(mCamera->translation()));
	mTranslation.set(-(state.pos-mCamera->predictedPos(transf)), mDuration); // Use the translation as adjustement over rotation 
																			 // as it is not as constrained
	mScale.set(glm::vec3(1.0f)); // Scale is not taken into account yet
}