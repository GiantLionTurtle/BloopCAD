
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <actions/action.hpp>
#include <graphics_utils/Camera.hpp>
#include <utils/animatable.hpp>
#include <geometry/geometry_3d/plane_abstr.hpp>

class moveCamera_action : public action {
private:
	Camera_ptr mCamera;
	CameraState mInitState, mTargetState;
	bool mStarted;
	animatable<glm::quat> mRotation;
	animatable<glm::vec3> mTranslation;
	animatable<glm::vec3> mScale;
	glm::vec3 mFinalOrientation;
	long mDuration;
public:
	moveCamera_action(Camera_ptr cam, CameraState target, long duration_ms);

	static std::shared_ptr<action> create_from_facingPlane(	geom_3d::plane_abstr* toFace, float dist_to_plane, 
															CameraState const& camSt, Camera_ptr cam);
	static std::shared_ptr<action> create_from_facingPlane(geom_3d::plane_abstr* toFace, float dist_to_plane, Camera_ptr cam);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
private:
	bool move_Camera();
	void compute_animatables(CameraState const& state);
};

#endif