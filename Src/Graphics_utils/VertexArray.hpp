
#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

/*
	@class VertexArray describes an abstraction of openGL's vertex arrays
	@warning : Make sure that every function is called whithin the same openGL context
*/
class VertexArray {
private:
	unsigned int mRendererID; // The openGL handle to the vertex array
public:
	/*
		@function VertexArray creates a VertexArray object
	*/
	VertexArray();
	/*
		@function ~VertexArray destroys a VertexArray object and cleanly handle the internal openGL stuff
	*/
	~VertexArray();

	/*
		@function add buffer adds a set of vertices to the vertex array

		@param vb : 	A VertexBuffer containing vertices data
		@param layout : A VertexBufferLayout describing the data of the VertexBuffer
	*/
	void add_buffer(VertexBuffer const& vb, VertexBufferLayout const& layout);

	/*
		@function bind activates the VertexArray in openGL so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the VertexArray in openGL so that it can't be used
	*/
	void unbind() const;

	/*
		@function id

		@return : The openGL handle to the vertex array
	*/
	unsigned int id() const { return mRendererID; }
};

#endif
