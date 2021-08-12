
#include "Camera.hpp"
#include <utils/DebugUtils.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/vector_angle.hpp>

void print_state(CameraState const& st)
{
	std::cout<<glm::to_string(st.pos)<<" - "<<glm::to_string(st.right)<<" - "<<glm::to_string(st.up);
}
std::ostream& operator<<(std::ostream &os, CameraState const& st)
{
	os<<glm::to_string(st.pos)<<" - "<<glm::to_string(st.right)<<" - "<<glm::to_string(st.up);
	return os;
}
bool operator !=(CameraState const& st1, CameraState const& st2) 
{
	return st1.pos != st2.pos || st1.right != st2.right|| st1.up != st2.up;
}
bool operator ==(CameraState const& st1, CameraState const& st2) 
{
	return st1.pos == st2.pos && st1.right == st2.right && st1.up == st2.up;
}

Camera::Camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, glm::vec2 viewport_):
	mInternalPos(cartesianCoords),
	mInternalTarget(target),
	mOrientation(glm::vec3(0.0f, 0.0f, 0.0f)),
	mZoom(1.0f),
	mFOV(FOV_),
	mNearPlane(0.1),
	mFarPlane(100),
	mViewport(viewport_),
	mInternalFront(0.0f, 0.0f, -1.0f),
	mTransformation{glm::vec3(0.0f, 0.0f, 0.0f)/*No translation*/, 
		glm::vec3(1.0f, 1.0f, 1.0f)/*Scale of 100%*/, 
		glm::quat(1.0f, 0.0f, 0.0f, 0.0f)/*No rotation with unit quaternion*/},
	mRequire_update(false)
{

}
Camera::Camera():
	mInternalPos(glm::vec3(0.0)),
	mInternalTarget(glm::vec3(0.0)),
	mOrientation(glm::vec3(0.0f, 0.0f, 0.0f)),
	mZoom(1.0f),
	mFOV(0.0f),
	mNearPlane(0.1),
	mFarPlane(100),
	mViewport(glm::vec2(0.0)),
	mInternalFront(0.0f, 0.0f, -1.0f),
	mTransformation{glm::vec3(0.0f, 0.0f, 0.0f)/*No translation*/, 
		glm::vec3(1.0f, 1.0f, 1.0f)/*Scale of 100%*/, 
		glm::quat(1.0f, 0.0f, 0.0f, 0.0f)/*No rotation with unit quaternion*/},
	mRequire_update(false)
{

}

glm::vec2 Camera::world_to_screen(glm::vec3 worldpos) const
{
	glm::vec4 onscreen_ndc = mvp() * glm::vec4(worldpos, 1.0f);
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, viewport().y, 0.0f));
	return onscreen;
}

glm::vec3 Camera::predictedPos(transform trans) const
{
	return model_inv(trans) * glm::vec4(mInternalPos, 1.0f);
}

void Camera::set_orientation(glm::vec3 orient)
{
	mOrientation = orient;
	orientation_to_rotation(mOrientation, mTransformation.rotation);
	mRequire_update = true; 
}

void Camera::copy(Camera_ptr other)
{
	mInternalPos = other->mInternalPos;
	mInternalTarget = other->mInternalTarget;
	mOrientation = other->mOrientation;

	mZoom = other->mZoom;
	mFOV = other->mFOV;
	mViewport = other->mViewport;

	mTransformation.rotation = other->mTransformation.rotation;
	mTransformation.translation = other->mTransformation.translation;
	mTransformation.scale = other->mTransformation.scale;
	mRequire_update = true;
}
void Camera::update(bool silent)
{
	if(!silent)
		mRequire_update = false;
	mModel 		= model(mTransformation);
	mView 		= glm::lookAt(mInternalPos, mInternalPos + mInternalFront, glm::vec3(0.0f, 1.0f, 0.0f));
	mProjection =  glm::perspective(mFOV, aspectRatio(), mNearPlane, mFarPlane);
	mMVP 		=  projection() * view() * model(); // The order of multiplication makes this slightly awkward but model is first
	mModel_inv = model_inv(mTransformation);

	mFront 	= glm::normalize(model_inv() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));  // Apply the inverse transform to the "real" front vector
	mRight 	= glm::normalize(model_inv() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));  // Apply the inverse transform to the "real" right vector
	mUp 	=  glm::normalize(model_inv() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));  // Apply the inverse transform to the "real" up vector

	mPos 	= model_inv() * glm::vec4(mInternalPos, 1.0f); // Apply the inverse transform to the "real" position
	mTarget = model_inv() * glm::vec4(mInternalTarget, 1.0f); // Apply the inverse transform to the "real" target
}

bool Camera::flipped() const
{
	// The Camera is flipped it's rotation around the x axis is between 90 and 270 degrees
	float mod_angle = std::fmod(mOrientation.x, M_PI * 2.0f);
	if(mod_angle < 0)
		mod_angle += M_PI * 2.0f;

	return (mod_angle > M_PI / 2.0f && mod_angle < M_PI * 3.0f / 2.0f);
}

bool Camera::flipped(CameraState state_)
{
	return state_.up.y < 0.0f;
}

CameraState Camera::state() const 
{
	return { pos(), right(), up() };
}

void Camera::orientation_to_rotation(glm::vec3 const& orientation, glm::quat& quaternion)
{
	// Create the quaternion 
	quaternion = 	glm::angleAxis(orientation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * 
					glm::angleAxis(orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::cast_ray(glm::vec2 screenPos, bool input_NDC/* = false*/)
{
	float screen_dist = 1.0f / std::tan(FOV() / 2.0f);
	float half_screenWidth = aspectRatio();
	glm::vec3 pos_on_screen;
	if(input_NDC) {
		pos_on_screen = glm::vec3(	map((float)screenPos.x, -1.0f, 1.0f, -half_screenWidth, half_screenWidth),
									screenPos.y, 
									-screen_dist);
	} else {
		pos_on_screen = glm::vec3(	map((float)screenPos.x, 0.0f, viewport().x, -half_screenWidth, half_screenWidth),
									map((float)screenPos.y, viewport().y, 0.0f, -1.0f, 1.0f), 
									-screen_dist);
	}
	return model_inv() * glm::vec4(glm::normalize(pos_on_screen), 0.0f);
}
glm::vec2 Camera::screen_angle(glm::vec2 screenPos, bool input_NDC)
{
	float screen_dist = 1.0f / std::tan(FOV() / 2.0f);
	float half_screenWidth = aspectRatio();
	glm::vec2 pos_on_screen;
	if(input_NDC) {
		pos_on_screen = glm::vec2(	map((float)screenPos.x, -1.0f, 1.0f, -half_screenWidth, half_screenWidth),
									screenPos.y);
	} else {
		pos_on_screen = glm::vec2(	map((float)screenPos.x, 0.0f, viewport().x, -half_screenWidth, half_screenWidth),
									map((float)screenPos.y, viewport().y, 0.0f, -1.0f, 1.0f));
	}
	return glm::vec2(std::atan2(pos_on_screen.x, screen_dist), std::atan2(pos_on_screen.y, screen_dist));
}
void Camera::get_alignedPlaneVectors(Geom3d::Plane_abstr* pl, glm::vec3& right, glm::vec3& up, bool allow_inversion)
{
	get_alignedPlaneVectors(state(), pl, right, up, allow_inversion);
}
void Camera::get_alignedPlaneVectors(CameraState cst, Geom3d::Plane_abstr* pl, glm::vec3& right, glm::vec3& up, bool allow_inversion)
{
	float dot_right_v 	= glm::dot(cst.right, pl->v()); // How similar is the camRight to the v vector?
	float dot_up_v 		= glm::dot(cst.up, pl->v()); // How similar is the camUp to the v vector?
	float dot_right_w 	= glm::dot(cst.right, pl->w());
	float dot_up_w 		= glm::dot(cst.up, pl->w());

	// Decide which of the Camera vectors will be assigned to which of the plane's vectors
	if(std::abs(dot_right_w) > std::abs(dot_right_v) && (std::abs(dot_right_w) > std::abs(dot_up_w) || std::abs(dot_right_v) < std::abs(dot_up_v))) {
		right 	= pl->w() * (dot_right_w < 0.0f && allow_inversion 	? -1.0f : 1.0f); // Invert it or not
		up 		= pl->v() * (dot_up_v < 0.0f && allow_inversion 	? -1.0f : 1.0f); // Invert it or not
	} else {
		right 	= pl->v() * (dot_right_v < 0.0f && allow_inversion 	? -1.0f : 1.0f); // Invert it or not
		up 		= pl->w() * (dot_up_w < 0.0f && allow_inversion 	? -1.0f : 1.0f); // Invert it or not
	}
}

glm::mat4 Camera::model(transform transf) const
{
	// The order matters!
	return 
	glm::toMat4(transf.rotation) *
	glm::translate(glm::mat4(1.0f), transf.translation) * 
	glm::scale(glm::mat4(1.0f), transf.scale);
}
glm::mat4 Camera::model_inv(transform transf) const
{
	// The order matters! - it is the inverse of the model's order
	return 
	glm::scale(glm::mat4(1.0f), 1.0f/transf.scale) * 
	glm::translate(glm::mat4(1.0f), -transf.translation) *
	glm::toMat4(glm::inverse(transf.rotation));
}