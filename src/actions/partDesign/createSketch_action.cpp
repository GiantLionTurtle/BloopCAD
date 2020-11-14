
#include "createSketch_action.hpp"

createSketch_action::createSketch_action(std::shared_ptr<plane_abstract> sketchPlane, std::shared_ptr<part> target):
	mSketchPlane(sketchPlane),
	mTarget(target),
	mSketch(nullptr)
{

}

void createSketch_action::do_work()
{
	if(mSketch) {
		
	} else {
		mSketch = std::shared_ptr<sketch>(new sketch(mSketchPlane, mTarget));
	}
}
void createSketch_action::undo_work()
{
	if(mSketch) {
		
	}
}