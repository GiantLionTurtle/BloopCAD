
#ifndef LINE_HPP_
#define LINE_HPP_

#include <entities/entity.hpp>

/*
	@class line_abstract is an abstract representation of a line
*/
class line_abstract {
protected:
	glm::vec3 mPointA, mPointB; // Both points that describe the line in 3d space
public:
	/*
		@line_abstract creates a line_abstract

		@param ptA : The start point of the line
		@param ptB : The end point of the line
	*/
	line_abstract(glm::vec3 ptA, glm::vec3 ptB): mPointA(ptA), mPointB(ptB) {}

	/*
		@function pointA is a getter for the start point of the line

		@return : The start point of the line
	*/
	glm::vec3 pointA() const { return mPointA; }
	void set_pointA(glm::vec3 ptA) { mPointA = ptA; }
	/*
		@function pointB is a getter for the end point of the line

		@return : The end point of the line
	*/
	glm::vec3 pointB() const { return mPointB; }
	void set_pointB(glm::vec3 ptB) { mPointB = ptB; }
};

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
	std::shared_ptr<vertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
	std::shared_ptr<shader> 		mShader, mSelectionShader; // Both shaders to draw the line and it's selection 
public:
	/*
		@function line creates a line 

		@param baseLine : The underlying line of the graphical entity
	*/
	line(line_abstract const& baseLine);

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