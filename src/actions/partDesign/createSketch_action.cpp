
#include "createSketch_action.hpp"

createSketch_action::createSketch_action(std::shared_ptr<plane_abstract> sketchPlane, std::shared_ptr<part> target):
	mSketchPlane(sketchPlane),
	mTarget(target),
	mSketch(nullptr)
{
	mSketch = std::shared_ptr<sketch>(new sketch(mSketchPlane, mTarget.get()));
	mSketch->set_exists(false); // It should not exist until the action is consumed
	mTarget->add_sketch(mSketch);
}

void createSketch_action::do_work()
{
	mSketch->set_exists(true); // The sketch now exists
}
void createSketch_action::undo_work()
{
	mSketch->set_exists(false); // The sketch now doesn't exist, it will not be saved, but if the action is re-done it is still available
}