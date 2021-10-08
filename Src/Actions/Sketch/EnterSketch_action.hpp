
#ifndef ENTERSKETCHDESIGN_ACTION_HPP_
#define ENTERSKETCHDESIGN_ACTION_HPP_

#include <Actions/Common/SwitchWorkspace_action.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Drawables/3d/Plane.hpp>

#include <memory>

/*
	@class EnterSketch_action: Allows to enter in sketch the Sketch workspace,
	set the workspace target to a specific sketche and set the camera to face the sketch

	@parent SwitchWorkspace_action
*/
class EnterSketch_action : public SwitchWorkspace_action {
private:
	Sketch* mTarget; // The sketch that will be edited once in Sketch workspace
public:
	/*
		@constructor EnterSketch_action

		@param target		The sketch to enter/edit
		@param set_camera	If the camera of the new workspace should be set to be at
							the same state then the camera of the workspace that is left
	*/
	EnterSketch_action(Sketch* target, bool set_camera = false);

	/*
		@function do_work switches to Sketch workspace (mainly uses SwitchWorkspace_action's function)

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);

	/*
		Note: undo_work is not overrided from SwitchWorkspace_action
	*/
};

#endif