
#include "vertexBuffer.hpp"

#include "GLCall.hpp"

vertexBuffer::vertexBuffer(void const* data, unsigned int size_):
	mSize(size_)
{
	// Create the buffer
	GLCall(glGenBuffers(1, &mRendererID));

	// Fill the buffer
	bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size_, data, GL_STATIC_DRAW)); // The draw mode might become an option eventually
}

vertexBuffer::~vertexBuffer()
{
	// Destroy the buffer
	GLCall(glDeleteBuffers(1, &mRendererID));
}

void vertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}
void vertexBuffer::unbind() const
{
#ifndef RELEASE_MODE // No unecesary bind in release, but practical for debugging
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
#endif
}