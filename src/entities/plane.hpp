
#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "entity.hpp"
#include "plane_abstract.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <iostream>

/*
	@class plane describes a plane drawn on screen
	@parent : entity
	@parent : plane_abstract
*/
class plane : public entity, public plane_abstract {
private:
	const glm::vec3 mColorA = glm::vec3(0.34f, 0.17f, 0.05f), 	// Both colors of a plane, depending on the side from which it is observed
	mColorB = glm::vec3(0.15f, 0.0f, 0.25f); 					// Maybe it should be an option

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6], mOutlineIndices[8]; // The order in which to draw said vertices

	std::shared_ptr<vertexBuffer> 	mVB; // Handle to the vertices on the gpu
	std::shared_ptr<vertexArray> 	mVA; // Handle to the configuration on the gpu
	std::shared_ptr<indexBuffer>	mIB, mOutlineIB; // Handle to the indices to draw the rectangle on the gpu
	std::shared_ptr<shader> 		mShader, mSelectionShader; // Both normal and selection shaders
public:
	/*
		@function plane creates a plane 

		@param plane_ : 				The underlaying plane abstraction
		@param label [defaults to ""] : The label of the plane (not supported yet)
	*/
	plane(plane_abstract const& plane_, std::string const& label = "");
protected:
	/*
		@function draw_impl draws the plane on screen

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(std::shared_ptr<camera> cam, int frame);
	/*
		@function draw_selection_impl draws the plane in plain color on the selection buffer

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_selection_impl(std::shared_ptr<camera> cam, int frame);

	void init_buffers();
};

#endif