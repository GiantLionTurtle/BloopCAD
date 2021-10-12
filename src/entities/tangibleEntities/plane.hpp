
#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "tangibleEntity.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <iostream>

/*
	@class plane describes a plane drawn on screen
	@parent : entity
	@parent : plane_abstract
*/
class plane : public tangibleEntity, public geom_3d::plane_abstr {
private:
	const glm::vec3 mColorA = glm::vec3(1.0f, 0.35f, 0.90f), 	// Both colors of a plane, depending on the side from which it is observed
					mColorB = glm::vec3(0.35f, 1.0f, 0.78f); 					// Maybe it should be an option

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6];
public:
	/*
		@function plane creates a plane 

		@param plane_ : 				The underlaying plane abstraction
		@param label [defaults to ""] : The label of the plane (not supported yet)
	*/
	plane(plane_abstr const& plane_);
	virtual ~plane() {}

	virtual void init();

	virtual int selection_rank() { return 10; }

	virtual void update_VB();
protected:
	/*
		@function draw_impl draws the plane on screen

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame);
	
	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	void init_buffers();
};

#endif