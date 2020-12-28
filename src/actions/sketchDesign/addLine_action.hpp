
#ifndef ADDLINE_ACTION_HPP_
#define ADDLINE_ACTION_HPP_

#include <actions/action.hpp>
#include <entities/forward_entities.hpp>

#include <glm/glm.hpp>

#include <memory>

class document; // Declaration of document needed to declare pointer type

/*
	@class addLine_action is an action to add a line to a sketch, given two points, a sketch and a document
	@parent : action
*/
class addLine_action : public action {
private:
	std::shared_ptr<line> mLine; // The created line
	std::shared_ptr<sketch> mSketch; // The containing sketch
	document* mDoc; // The containing document
public:
	/*
		@function addLine_action creates the action and the actual line

		@param pointA : 	The first point of the line
		@param pointB : 	The second point of the line
		@param sketch_ : 	The containing sketch
		@param document : 	The containing document 
	*/
	addLine_action(glm::vec3 const& pointA, glm::vec3 const& pointB, std::shared_ptr<sketch> sketch_, document* doc);

	/*
		@function do_work activates the created line
	*/
	virtual bool do_work();
	/*
		@function undo_work desactivates the created line
	*/
	virtual bool undo_work();
};

#endif