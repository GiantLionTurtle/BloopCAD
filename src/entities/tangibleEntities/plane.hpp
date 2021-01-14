
#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "tangibleEntity.hpp"
#include <entities/geometry/plane_abstract.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <iostream>

/*
	@class plane describes a plane drawn on screen
	@parent : entity
	@parent : plane_abstract
*/
class plane : public tangibleEntity, public plane_abstract {
private:
	const glm::vec3 mColorA = glm::vec3(0.34f, 0.17f, 0.05f), 	// Both colors of a plane, depending on the side from which it is observed
	mColorB = glm::vec3(0.15f, 0.0f, 0.25f); 					// Maybe it should be an option

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6];
public:
	/*
		@function plane creates a plane 

		@param plane_ : 				The underlaying plane abstraction
		@param label [defaults to ""] : The label of the plane (not supported yet)
	*/
	plane(plane_abstract const& plane_);
	virtual ~plane() {}

	virtual int selection_rank() { return 10; }

	virtual void update_VB();
protected:
	/*
		@function draw_impl draws the plane on screen

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame);
	/*
		@function draw_selection_impl draws the plane in plain color on the selection buffer

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_selection_impl(camera_ptr cam, int frame);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	void init_buffers();
};

#endif