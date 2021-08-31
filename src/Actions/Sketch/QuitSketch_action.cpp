
#include "QuitSketch_action.hpp"
#include <document.hpp>

QuitSketch_action::QuitSketch_action(Sketch* target)
{

}

bool QuitSketch_action::do_work(document* caller)
{
	LOG_WARNING("?_? what's going on here");
	return true;
}

bool QuitSketch_action::undo_work(document* caller)
{
	LOG_WARNING("?_? what's going on here"); // I don't remember right now if it is used
	return true;
}