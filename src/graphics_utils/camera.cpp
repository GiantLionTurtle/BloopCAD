
#include "camera.hpp"
#include <utils/errorLogger.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/vector_angle.hpp>

void print_state(camState const& st)
{
	std::cout<<glm::to_string(st.pos)<<" - "<<glm::to_string(st.right)<<" - "<<glm::to_string(st.up);
}
std::ostream& operator<<(std::ostream &os, camState const& st)
{
	os<<glm::to_string(st.pos)<<" - "<<glm::to_string(st.right)<<" - "<<glm::to_string(st.up);
	return os;
}
bool operator !=(camState const& st1, camState const& st2) 
{
	return st1.pos != st2.pos || st1.right != st2.right|| st1.up != st2.up;
}
bool operator ==(camState const& st1, camState const& st2) 
{
	return st1.pos == st2.pos && st1.right == st2.right && st1.up == st2.up;
}

camera::camera(glm::vec3 const& cartesianCoords, glm::vec3 const& target, float FOV_, glm::vec2 viewport_):
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

glm::vec3 camera::predictedPos(transform trans) const
{
	return model_inv(trans) * glm::vec4(mInternalPos, 1.0f);
}

void camera::set_orientation(glm::vec3 orient)
{
	mOrientation = orient;
	orientation_to_rotation(mOrientation, mTransformation.rotation);
	mRequire_update = true; 
}

void camera::set(camera_ptr other)
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
void camera::update()
{
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

bool camera::flipped() const
{
	// The camera is flipped it's rotation around the x axis is between 90 and 270 degrees
	float mod_angle = std::fmod(mOrientation.x, M_PI * 2.0f);
	if(mod_angle < 0)
		mod_angle += M_PI * 2.0f;

	return (mod_angle > M_PI / 2.0f && mod_angle < M_PI * 3.0f / 2.0f);
}

bool camera::flipped(camState state_)
{
	return state_.up.y < 0.0f;
}

camState camera::state() const 
{
	return { pos(), right(), up() };
}

void camera::orientation_to_rotation(glm::vec3 const& orientation, glm::quat& quaternion)
{
	// Create the quaternion 
	quaternion = 	glm::angleAxis(orientation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * 
					glm::angleAxis(orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 camera::cast_ray(glm::vec2 screenPos, bool input_NDC/* = false*/)
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

glm::mat4 camera::model(transform transf) const
{
	// The order matters!
	return 
	glm::toMat4(transf.rotation) *
	glm::translate(glm::mat4(1.0f), transf.translation) * 
	glm::scale(glm::mat4(1.0f), transf.scale);
}
glm::mat4 camera::model_inv(transform transf) const
{
	// The order matters! - it is the inverse of the model's order
	return 
	glm::scale(glm::mat4(1.0f), 1.0f/transf.scale) * 
	glm::translate(glm::mat4(1.0f), -transf.translation) *
	glm::toMat4(glm::inverse(transf.rotation));
}