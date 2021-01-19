
#ifndef LINE_HPP_
#define LINE_HPP_

#include "tangibleEntity.hpp"
#include <entities/geometry/line_abstract.hpp>
#include "point.hpp"

/*
	@class line describes a graphical line
	@parent : entity
	@parent : line_abstract
*/
class line : public tangibleEntity, public line_abstract {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Line color
	glm::vec3 mVertices[2]; // The vertices describing the line
public:
	/*
		@function line creates a line 

		@param baseLine : The underlying line of the graphical entity
	*/
	line(line_abstract const& baseLine);
	virtual ~line() {}

	virtual int selection_rank() { return 7; }

	virtual void move(glm::vec3 from, glm::vec3 to);
	virtual void set_constant();
	virtual void set_tmpConstant(bool const_);

	virtual void update_VB();
protected:
	/*
		@function draw_impl draws the line on screen

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	virtual void post_set_update();
};

#endif