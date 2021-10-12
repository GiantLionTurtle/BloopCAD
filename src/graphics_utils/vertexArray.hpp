
#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

#include "vertexBuffer.hpp"
#include "vertexBufferLayout.hpp"

/*
	@class vertexArray describes an abstraction of openGL's vertex arrays
	@warning : Make sure that every function is called whithin the same openGL context
*/
class vertexArray {
private:
	unsigned int mRendererID; // The openGL handle to the vertex array
public:
	/*
		@function vertexArray creates a vertexArray object
	*/
	vertexArray();
	/*
		@function ~vertexArray destroys a vertexArray object and cleanly handle the internal openGL stuff
	*/
	~vertexArray();

	/*
		@function add buffer adds a set of vertices to the vertex array

		@param vb : 	A vertexBuffer containing vertices data
		@param layout : A vertexBufferLayout describing the data of the vertexBuffer
	*/
	void add_buffer(vertexBuffer const& vb, vertexBufferLayout const& layout);

	/*
		@function bind activates the vertexArray in openGL so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the vertexArray in openGL so that it can't be used
	*/
	void unbind() const;

	/*
		@function id

		@return : The openGL handle to the vertex array
	*/
	unsigned int id() const { return mRendererID; }
};

#endif
