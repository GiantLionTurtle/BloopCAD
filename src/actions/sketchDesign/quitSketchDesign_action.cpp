
#include "quitSketchDesign_action.hpp"
#include <document.hpp>

quitSketchDesign_action::quitSketchDesign_action(Sketch* target)
{

}

bool quitSketchDesign_action::do_work(document* caller)
{
	LOG_WARNING("?_? what's going on here");
	return true;
}

bool quitSketchDesign_action::undo_work(document* caller)
{
	LOG_WARNING("?_? what's going on here"); // I don't remember right now if it is used
	return true;
}