
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Graphics_utils/CameraState.hpp>
#include <Utils/Animatable.hpp>

namespace Geom3d {
	class Plane_abstr;
}
class Camera;

/*
	@class MoveCamera_action moves the camera from a state to another over 
	several ticks, it is currently linear

	@parent Action
*/
class MoveCamera_action : public Action {
private:
	Camera* mCamera;						// The camera to move
	CameraState mInitState, mTargetState;	// Both states used to compute the path
	bool mStarted;							// If a move is in progress
	Animatable<glm::quat> mRotation;		// A rotation animatable that goes from initial to end rotation
	Animatable<glm::vec3> mTranslation;		// A translation animatable that goes from initial to end translation
	Animatable<glm::vec3> mScale;			// A scale animatable that goes from initial to end scale (not very used.. (as in not))
	glm::vec3 mFinalOrientation;			// Finale view vector
	long mDuration;							// Length for the movement in milliseconds 
											// (it might be changed to a velocity setting, rather, in the future)
public:
	/*
		@constructor MoveCamera_action

		@param cam 		The camera that must be animated
		@target 		The way the camera must be oriented at the end
		@duration_ms 	The duration of the movement in millisedonc
	*/
	MoveCamera_action(Camera* cam, CameraState target, long duration_ms);

	/*
		@staticfunction create_from_facingPlane creates the movement from a plane that must be in
		front of the camera in the end. (Used to move the camera in front of sketches)

		@param toFace 			The plane that the camera tries to face
		@param dist_to_plane 	The final distance between the camera and the plane
		@param camSt 			The state the camera must have at the start of the movement
		@param cam 				The camera to move 
	*/
	static Action_ptr create_from_facingPlane(Geom3d::Plane_abstr* toFace, float dist_to_plane, 
															CameraState const& camSt, Camera* cam);
	/*
		@staticfunction create_from_facingPlane creates the movement from a plane that must be in
		front of the camera in the end. (Used to move the camera in front of sketches). It is identical to 
		the other version of this function except the initial state is the camera's current state

		@param toFace 			The plane that the camera tries to face
		@param dist_to_plane 	The final distance between the camera and the plane
		@param camSt 			The state the camera must have at the start of the movement
		@param cam 				The camera to move
	*/ 
	static Action_ptr create_from_facingPlane(Geom3d::Plane_abstr* toFace, float dist_to_plane, Camera* cam);

	/*
		@function do_work moves the camera along the path so that it takes the specified
		amount of time in total

		@param caller: The document calling the function
		@return False for the duration of the move, then True
	*/
	virtual bool do_work(Document* caller);
	/*
		@function do_work moves the camera along the reverse path so that it takes 
		the specified amount of time in total

		@param caller: The document calling the function
		@return False for the duration of the move, then True
	*/
	virtual bool undo_work(Document* caller);
private:
	/*
		@function move_camera displaces the camera along any path (the computed path or it's reverse)

		@return True if the all the animatable have reached their final state
		(which they should all do at the same time)
	*/
	bool move_camera();
	/*
		@function compute_animatables breaks down movement into it's rotation, translation and scale 
		movements.

		@param state The target state (wether it has to compute the animatables to go toward the end or the 
		begining of the path)
		@note This is the core of the class
	*/
	void compute_animatables(CameraState const& state);
};

#endif