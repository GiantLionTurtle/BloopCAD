
#ifndef LINE_HPP_
#define LINE_HPP_

#include <entities/entity.hpp>
#include "line_abstract.hpp"
#include "point.hpp"

/*
	@class line describes a graphical line
	@parent : entity
	@parent : line_abstract
*/
class line : public entity, public line_abstract {
private:	
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Line colors

	glm::vec3 mVertices[2]; // The vertices describing the line
	
	std::shared_ptr<vertexBuffer> 	mVB; // The vertex buffer to contain the vertices on the gpu
	bool mRequire_VBUpdate;
	std::shared_ptr<vertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
	std::shared_ptr<shader> 		mShader, mSelectionShader; // Both shaders to draw the line and it's selection (curently the normal shader is used for selection with 0 feather)
public:
	/*
		@function line creates a line 

		@param baseLine : The underlying line of the graphical entity
	*/
	line(line_abstract const& baseLine);

	virtual void set_pointA(point_abstract ptA);
	virtual void set_pointB(point_abstract ptB);

	void update_VB();
protected:
	/*
		@function draw_impl draws the line on screen

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(std::shared_ptr<camera> cam, int frame);
	/*
		@function draw_selection_impl draws the line in plain color on the selection buffer

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_selection_impl(std::shared_ptr<camera> cam, int frame);
};

#endif