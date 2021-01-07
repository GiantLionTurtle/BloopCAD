
#include "addLine_action.hpp"
#include <entities/sketch.hpp>
#include <entities/tangibleEntities/line.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>
#include <document.hpp>

addLine_action::addLine_action(glm::vec2 const& pointA, glm::vec2 const& pointB, std::shared_ptr<sketch> sketch_, document* doc):
	mSketch(sketch_)
{
	doc->make_glContext_current(); // The openGL context must be set to our rendering context, not an obscure gtk context
	mLine = std::make_shared<line>(line_abstract(	std::make_shared<sketchPoint>(sketch_->basePlane(), pointA), 
													std::make_shared<sketchPoint>(sketch_->basePlane(), pointB))); // Create said line
	mSketch->add(mLine);
	mLine->set_exists(false); // It should not exist until the action is consummed
}
addLine_action::addLine_action(std::shared_ptr<point_abstract> pointA, std::shared_ptr<point_abstract> pointB, std::shared_ptr<sketch> sketch_, document* doc):
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
	return true;
}
bool addLine_action::undo_work()
{
	mLine->set_exists(false); // The line now doesn't exist, but is still available if needed
	return true;
}