
#include "EnterSketch_action.hpp"
#include <Workspaces/Document.hpp>
#include <Bloop.hpp>

EnterSketch_action::EnterSketch_action(Sketch* target, bool set_camera):
	SwitchWorkspace_action(Bloop::workspace_types::SKETCH, set_camera),
	mTarget(target)
{

}

bool EnterSketch_action::do_work(Document* caller)
{
	caller->window()->sketchWorkspace()->set_target(mTarget); // The Sketch workspace now edits the target sketch
	SwitchWorkspace_action::do_work(caller); // Do switch the workspace
	return true;
}