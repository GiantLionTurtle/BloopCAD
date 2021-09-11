
#include "EnterSketch_action.hpp"
#include <Workspaces/Document.hpp>
#include <Bloop.hpp>

EnterSketch_action::EnterSketch_action(Sketch* target, bool set_Camera):
	SwitchWorkspace_action(Bloop::workspace_types::SKETCH, set_Camera),
	mTarget(target)
{

}

bool EnterSketch_action::do_work(Document* caller)
{
	caller->window()->sketchWorkspace()->set_target(mTarget);
	SwitchWorkspace_action::do_work(caller);
	return true;
}

bool EnterSketch_action::undo_work(Document* caller)
{
	SwitchWorkspace_action::undo_work(caller);
	return true;
}