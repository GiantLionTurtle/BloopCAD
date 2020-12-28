
#include "addLine_action.hpp"
#include <entities/sketch.hpp>
#include <entities/line.hpp>
#include <utils/errorLogger.hpp>
#include <document.hpp>

addLine_action::addLine_action(glm::vec3 const& pointA, glm::vec3 const& pointB, std::shared_ptr<sketch> sketch_, document* doc):
	mSketch(sketch_)
{
	doc->make_glContext_current(); // The openGL context must be set to our rendering context, not an obscure gtk context
	mLine = std::shared_ptr<line>(new line(line_abstract(pointA, pointB))); // Create said line
	mSketch->add(mLine);
	mLine->set_exists(false); // It should not exist until the action is consummed
}

bool addLine_action::do_work()
{
	mLine->set_exists(true); // The line now exists
	return false;
}
bool addLine_action::undo_work()
{
	mLine->set_exists(false); // The line now doesn't exist, but is still available if needed
	return true;
}