
#ifndef QUITSKETCHDESIGN_ACTION_HPP_
#define QUITSKETCHDESIGN_ACTION_HPP_

#include <actions/action.hpp>
#include <Drawables/sketch.hpp>
#include <Drawables/tangibleEntities/plane.hpp>

#include <memory>

class quitSketchDesign_action : public action {
private:
	std::shared_ptr<plane> mPlane;
public:
	quitSketchDesign_action(sketch_ptr target);

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