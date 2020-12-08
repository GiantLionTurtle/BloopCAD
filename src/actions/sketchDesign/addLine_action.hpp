
#ifndef ADDLINE_ACTION_HPP_
#define ADDLINE_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/forward_entities.hpp>

#include <glm/glm.hpp>

#include <memory>

class document;

class addLine_action : public action {
private:
	std::shared_ptr<line> mLine;
	std::shared_ptr<sketch> mSketch;
	document* mDoc;
public:
	addLine_action(glm::vec3 const& pointA, glm::vec3 const& pointB, std::shared_ptr<sketch> sketch_, document* doc);

	virtual void do_work();
	virtual void undo_work();
};

#endif