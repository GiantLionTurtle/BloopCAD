
#ifndef ENTERSKETCHDESIGN_ACTION_HPP_
#define ENTERSKETCHDESIGN_ACTION_HPP_

#include <actions/common/switchWorkspace_action.hpp>
#include <entities/sketch.hpp>
#include <entities/tangibleEntities/plane.hpp>

#include <memory>

class enterSketchDesign_action : public switchWorkspace_action {
private:
	std::shared_ptr<plane> mPlane;
	sketch_ptr mTarget;
	entity_ptr mInitialTarget;
public:
	enterSketchDesign_action(sketch_ptr target, bool set_camera = false);

	/*
		@function do_work switches to desired workspace if it exists
	*/
	virtual bool do_work(document* caller);
	/*
		@function undo_work revert back to the initial workspace
	*/
	virtual bool undo_work(document* caller);
};

#endif