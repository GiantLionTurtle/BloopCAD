
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Graphics_utils/Camera.hpp>
#include <Utils/Animatable.hpp>
#include <Geom/Geom3d/Plane_abstr.hpp>

class MoveCamera_action : public Action {
private:
	Camera* mCamera;
	CameraState mInitState, mTargetState;
	bool mStarted;
	Animatable<glm::quat> mRotation;
	Animatable<glm::vec3> mTranslation;
	Animatable<glm::vec3> mScale;
	glm::vec3 mFinalOrientation;
	long mDuration;
public:
	MoveCamera_action(Camera* cam, CameraState target, long duration_ms);

	static Action_ptr create_from_facingPlane(	Geom3d::Plane_abstr* toFace, float dist_to_plane, 
															CameraState const& camSt, Camera* cam);
	static Action_ptr create_from_facingPlane(Geom3d::Plane_abstr* toFace, float dist_to_plane, Camera* cam);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
private:
	bool move_Camera();
	void compute_Animatables(CameraState const& state);
};

#endif