
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <actions/action.hpp>
#include <graphics_utils/camera.hpp>
#include <utils/animatable.hpp>
#include <entities/geometry/plane_abstract.hpp>

class moveCamera_action : public action {
private:
	camera_ptr mCamera;
	cameraState mInitState, mTargetState;
	bool mStarted;
	animatable<glm::quat> mRotation;
	animatable<glm::vec3> mTranslation;
	animatable<glm::vec3> mScale;
	glm::vec3 mFinalOrientation;
	long mDuration;
public:
	moveCamera_action(camera_ptr cam, cameraState target, long duration_ms);

	static std::shared_ptr<action> create_from_facingPlane(	plane_abstract_ptr toFace, float dist_to_plane, 
															cameraState const& camSt, camera_ptr cam);
	static std::shared_ptr<action> create_from_facingPlane(plane_abstract_ptr toFace, float dist_to_plane, camera_ptr cam);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
private:
	bool move_camera();
	void compute_animatables(cameraState const& state);
};

#endif