
#include "createSketch_action.hpp"

#include <utils/errorLogger.hpp>

createSketch_action::createSketch_action(plane_abstract_ptr sketchPlane, std::shared_ptr<part> target):
	mSketchPlane(sketchPlane),
	mTarget(target),
	mSketch(nullptr)
{
	mSketch = sketch_ptr(new sketch(mSketchPlane));
	mSketch->set_exists(false); // It should not exist until the action is consumed
	mTarget->add_sketch(mSketch);
}

bool createSketch_action::do_work()
{
	mSketch->set_exists(true); // The sketch now exists
	return true;
}
bool createSketch_action::undo_work()
{
	mSketch->set_exists(false); // The sketch now doesn't exist, it will not be saved, but if the action is re-done it is still available
	return true;
}