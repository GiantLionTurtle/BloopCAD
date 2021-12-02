
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

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "CameraState.hpp"
#include <Utils/Animatable.hpp>
#include <Utils/Debug_util.hpp>

#include <Geometry/3d/Plane_abstr.hpp>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <memory>

/*
	@struct transform describes the transformations of a model
*/
struct transform {
	glm::vec3 translation;
	glm::vec3 scale;
	glm::quat rotation;
};

/*
	@function print_states prints a CameraState to the console, for debugging

	@param st The state to print
*/
void print_state(CameraState const& st);
/*
	@function operator<< prints a CameraState to the console, for debugging

	@param os The output stream
	@param st The state to print
*/
std::ostream& operator<<(std::ostream &os, CameraState const& st);

/*
	@operator != makes a boolean comparison of each member of two states

	@param st1 The first state to compare
	@param st2 The second state to compare
	@return If they are not identical
*/
bool operator !=(CameraState const& st1, CameraState const& st2);
/*
	@operator != makes a boolean comparison of each member of two states

	@param st1 The first state to compare
	@param st2 The second state to compare
	@return If they are identical
*/
bool operator ==(CameraState const& st1, CameraState const& st2);

class Camera {
protected:
	// Since it's the model that is transformed by the mvp, it is good to keep
	// The "true" parameters of the camera AND the "virtual" ones

	// "True" parameters of the camera
	glm::vec3 mInternalPos, mInternalTarget; // Position of the Camera and the position it is looking at
	glm::vec3 mInternalFront; // The front of the vector (should go from mInternalPos to mInternalTarget)
	glm::vec3 mOrientation; // Rotation along x, y then z axis, this is not a quaternion, this orientation is spherical (since the camera is orbital)

	// Virtual parameters of the camera, they form a CameraState but with more data (some is redundant, like front)
	glm::vec3 mRight, mUp, mFront, mPos, mTarget;

	float mZoom; 		// The overall zoom (unused and not supported for now)
	float mFOV; 		// The field of view of the Camera
	float mNearPlane, mFarPlane;
	glm::vec2 mViewport;		// The viewport dimensions 
	transform mTransformation;		// The transformation of the model
	glm::mat4 mModel, mView, mProjection, mMVP, mModel_inv;

	bool mNeed_update;
public:
	/*
		@constructor Camera creates a Camera

		@param cartesianCoords 	The position of the Camera in the world
		@param target :				The position of the target of the Camera in the world
		@param FOV_ :				The field of view of the Camera
		@param viewport_ 			The dimensions the drawing area
	*/
	Camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, glm::vec2 viewport_);
	Camera();

	/*
		@functiongroup

		@function model
		@function model_inv
		@function view
		@function projection
		@function mvp
		@function pos
		@function target

		@note (computed in update so it *might* not be up to date)
	*/
	/*
		@function model

		@return The model transformation matrix 
	*/
	glm::mat4 model() const { return mModel; }
	/*
		@function model_inv

		@return The Camera transformation matrix 
		@note It is used to compute the "virtual" parameters of the Camera, for instantce, if the "true" 
				Camera position is multiplied with this matrix it gives the "virtual" position
	*/
	glm::mat4 model_inv() const { return mModel_inv; }
	/*
		@function view 

		@return The view matrix 
	*/
	glm::mat4 view() const { return mView; }
	/*
		@function projection

		@return The projection matrix 
	*/
	glm::mat4 projection() const { return mProjection; }
	/*
		@function mvp

		@return The conjonction of model, view and projection matrices 
	*/
	glm::mat4 mvp() const { return mMVP; };

	/*
		@function pos

		@return The position of the Camera in the world
	*/
	glm::vec3 pos() const { return mPos; }
	glm::vec3 predictedPos(transform trans) const;
	/*
		@function target

		@return The position of the target in the world
	*/
	glm::vec3 target() const { return mTarget; }

	glm::vec2 world_to_screen(glm::vec3 worldpos) const;
	glm::vec3 screen_to_world(glm::vec2 screen, Geom3d::Plane_abstr* support_pl);
	glm::vec2 screen_to_plane(glm::vec2 screen, Geom3d::Plane_abstr* support_pl);

	/*
		@function up

		@return The position of the up in the world
	*/
	glm::vec3 up() const { return mUp; }
	/*
		@function right

		@return The position of the right in the world
	*/
	glm::vec3 right() const { return mRight; }
	/*
		@function front

		@return The position of the front in the world 
	*/
	glm::vec3 front() const { return mFront; }
	
	/*
		@function orientation

		@return The orientation of the Camera
	*/
	glm::vec3 orientation() { return mOrientation; }
	/*
		@function set_orientation

		@return orient The new orientation
	*/
	void set_orientation(glm::vec3 orient);
	
	/*
		@function FOV

		@return The field of view of the Camera as a simple float
	*/
	float& FOV() { return mFOV; }
	void set_FOV(float aFOV) { mFOV = aFOV; set_need_update(); }

	/*
		@function zoom

		@return The zoom of the Camera
	*/
	float zoom() const { return mZoom; }
	void set_zoom(float aZoom) { mZoom = aZoom; set_need_update(); };

	float nearPlane() { return mNearPlane; }
	void set_nearPlane(float near) { mNearPlane = near; set_need_update(); }
	float farPlane() { return mFarPlane; }
	void set_farPlane(float far) { mFarPlane = far; set_need_update(); }

	glm::vec2 viewport() const { return mViewport; }
	void set_viewport(glm::vec2 dimensions) { mViewport = dimensions; set_need_update(); }
	/*
		@function aspectRatio

		@return The aspectRatio of the drawing area
	*/
	float aspectRatio() const { return mViewport.x / mViewport.y; }

	/*
		@function transformation

		@return The transformation of the model
	*/
	glm::vec3 translation() { return mTransformation.translation; }
	glm::vec3 scale() { return mTransformation.scale; }
	float fscale() { return mTransformation.scale.x; }
	glm::quat rotation() { return mTransformation.rotation; }
	transform transformation() { return mTransformation; }
	void set_transformation(transform transf) { mTransformation = transf; set_need_update(); }
	void set_translation(glm::vec3 tr) { mTransformation.translation = tr; set_need_update(); }
	void set_scale(glm::vec3 sc) { mTransformation.scale = sc; set_need_update(); }
	void set_rotation(glm::quat rot) { mTransformation.rotation = rot; set_need_update(); }

	glm::vec3 internalPos() { return mInternalPos; } 
	void set_internalPos(glm::vec3 aPos) { mInternalPos = aPos; set_need_update(); } 

	glm::vec3 internalFront() { return mInternalFront; } 
	void set_internalFront(glm::vec3 aFront) { mInternalFront = aFront; set_need_update(); } 

	/*
		@function set sets the Camera to have the same everything as another Camera
	*/
	void copy(Camera* other);

	void set_need_update() { mNeed_update = true; }
	bool need_update() const { return mNeed_update; }
	void update(bool silent = false);

	/*
		@function flipped 

		@return Whether or not the Camera is upside down
	*/
	bool flipped() const;
	static bool flipped(CameraState state_);

	CameraState state() const;

	static void orientation_to_rotation(glm::vec3 const& orientation, glm::quat& quaternion);

	glm::vec3 cast_ray(glm::vec2 screenPos, bool input_NDC = false);
	glm::vec2 screen_angle(glm::vec2 screenPos, bool input_NDC = false);
	void get_alignedPlaneVectors(Geom3d::Plane_abstr* pl, glm::vec3& right, glm::vec3& up, bool allow_inversion);
	static void get_alignedPlaneVectors(CameraState cst, Geom3d::Plane_abstr* pl, glm::vec3& right, glm::vec3& up, bool allow_inversion);
private:
	glm::mat4 model(transform transf) const;
	glm::mat4 model_inv(transform transf) const;
};

#endif
