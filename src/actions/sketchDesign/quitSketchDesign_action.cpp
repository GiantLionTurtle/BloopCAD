
#include "quitSketchDesign_action.hpp"
#include <document.hpp>

quitSketchDesign_action::quitSketchDesign_action(Sketch_ptr target):
	mPlane(nullptr)
{
	mPlane = std::dynamic_pointer_cast<plane>(target->basePlane());
}

bool quitSketchDesign_action::do_work(document* caller)
{
	if(mPlane) {
		mPlane->show();
	}
	return true;
}

bool quitSketchDesign_action::undo_work(document* caller)
{
	if(mPlane) {
		mPlane->hide();
	}
	return true;
}