
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <utils/animatable.hpp>

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
	@struct camState is a helper struct to help describe a camera at a given orientation
*/
struct camState {
	glm::vec3 pos, right, up;
};

void print_state(camState const& st);
bool operator !=(camState const& st1, camState const& st2);
bool operator ==(camState const& st1, camState const& st2);

class camera {
protected:
	glm::vec3 mPos, mTarget; // Position of the camera and the position it is looking at
	glm::vec3 mOrientation; // Rotation along x, y then z axis

	float mZoom; 		// The overall zoom (unused and not supported for now)
	float mFOV; 		// The field of view of the camera
	glm::vec2 mViewport;		// The viewport dimensions 
	transform mTransformation;		// The transformation of the model
public:
	/*
		@function camera creates a camera

		@param cartesianCoords : 	The position of the camera in the world
		@param target :				The position of the target of the camera in the world
		@param FOV_ :				The field of view of the camera
		@param viewport_ : 			The dimensions the drawing area
	*/
	camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, glm::vec2 viewport_);

	/*
		@function model

		@return : The model transformation matrix
	*/
	glm::mat4 model() const;
	/*
		@function model_inv

		@return : The camera transformation matrix
	*/
	glm::mat4 model_inv() const;
	/*
		@function view 

		@return : The view matrix
	*/
	glm::mat4 view() const;
	/*
		@function projection

		@return : The projection matrix
	*/
	glm::mat4 projection() const;
	/*
		@function mvp - helper

		@return : The conjonction of model, view and projection matrices
	*/
	glm::mat4 mvp() const;

	/*
		@function pos

		@return : The position of the camera in the world (obtained with model_inv and the "real" position)
	*/
	glm::vec3 pos() const;
	glm::vec3 predictedPos(transform trans) const;
	/*
		@function target

		@return : The position of the target in the world (obtained with model_inv and the "real" target)
	*/
	glm::vec3 target() const;

	/*
		@function up

		@return : The position of the up in the world (obtained with model_inv and the "real" up)
	*/
	glm::vec3 up() const;
	/*
		@function right

		@return : The position of the right in the world (obtained with model_inv and the "real" right)
	*/
	glm::vec3 right() const;
	/*
		@function front

		@return : The position of the front in the world (obtained with model_inv and the "real" front)
	*/
	glm::vec3 front() const;
	
	/*
		@function orientation

		@return : The orientation of the camera
	*/
	glm::vec3 orientation() { return mOrientation; }
	void set_orientation(glm::vec3 orient) { mOrientation = orient; orientation_to_rotation(mOrientation, mTransformation.rotation); }
	
	/*
		@function FOV

		@return : The field of view of the camera as a simple float
	*/
	float& FOV() { return mFOV; }
	/*
		@function zoom

		@return : The zoom of the camera
	*/
	float& zoom() { return mZoom; };

	glm::vec2& viewport() { return mViewport; };
	/*
		@function aspectRatio

		@return : The aspectRatio of the drawing area
	*/
	float aspectRatio() const { return mViewport.x / mViewport.y; }

	/*
		@function transformation

		@return : The transformation of the model
	*/
	transform& transformation() { return mTransformation; }

	/*
		@function set sets the camera to have the same everything as another camera
	*/
	void set(std::shared_ptr<camera> other);

	/*
		@function flipped 

		@return : Whether or not the camera is upside down
	*/
	bool flipped() const;

	camState state() const;

	static void orientation_to_rotation(glm::vec3 const& orientation, glm::quat& quaternion);
private:
	glm::mat4 model(transform transf) const;
	glm::mat4 model_inv(transform transf) const;
};

#endif
