
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <actions/action.hpp>
#include <graphics_utils/camera.hpp>
#include <utils/animatable.hpp>
#include <entities/geometry/plane_abstract.hpp>

class moveCamera_action : public action {
private:
	camera_ptr mCamera;
	camState mInitState, mTargetState;
	bool mStarted;
	animatable<glm::quat> mRotation;
	animatable<glm::vec3> mTranslation;
	animatable<glm::vec3> mScale;
	glm::vec3 mFinalOrientation;
	long mDuration;
public:
	moveCamera_action(camera_ptr cam, camState target, long duration_ms);

	static std::shared_ptr<action> create_from_facingPlane(	plane_abstract_ptr toFace, float dist_to_plane, 
															camState const& camSt, camera_ptr cam);
	static std::shared_ptr<action> create_from_facingPlane(plane_abstract_ptr toFace, float dist_to_plane, camera_ptr cam);

	virtual bool do_work();
	virtual bool undo_work();
private:
	bool move_camera();
	void compute_animatables(camState const& state);
};

#endif