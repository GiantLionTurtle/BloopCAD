
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <actions/action.hpp>
#include <graphics_utils/camera.hpp>
#include <utils/animatable.hpp>
#include <entities/geometry/plane_abstract.hpp>

class moveCamera_action : public action {
private:
	std::shared_ptr<camera> mCamera;
	camState mInitState, mTargetState;
	bool mStarted;
	animatable<glm::quat> mRotation;
	animatable<glm::vec3> mTranslation;
	animatable<glm::vec3> mScale;
	glm::vec3 mFinalOrientation;
	long mDuration;
public:
	moveCamera_action(std::shared_ptr<camera> cam, camState target, long duration_ms);

	static std::shared_ptr<action> create_from_facingPlane(	std::shared_ptr<plane_abstract> toFace, float dist_to_plane, 
															camState const& camSt, std::shared_ptr<camera> cam);
	static std::shared_ptr<action> create_from_facingPlane(std::shared_ptr<plane_abstract> toFace, float dist_to_plane, std::shared_ptr<camera> cam);

	virtual bool do_work();
	virtual bool undo_work();
private:
	bool move_camera();
	void compute_animatables(camState const& state);
};

#endif