
#include "addLine_action.hpp"
#include <entities/sketch.hpp>
#include <entities/line.hpp>
#include <utils/errorLogger.hpp>
#include <document.hpp>

addLine_action::addLine_action(glm::vec3 const& pointA, glm::vec3 const& pointB, std::shared_ptr<sketch> sketch_, document* doc):
	mSketch(sketch_)
{
	doc->make_glContext_current();
	mLine = line::from_2Points_ptr(pointA, pointB);
	mSketch->add(mLine);
	mLine->set_exists(false);
}

void addLine_action::do_work()
{
	mLine->set_exists(true);
}
void addLine_action::undo_work()
{
	mLine->set_exists(false);
}