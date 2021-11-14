
#include "VertexBuffer.hpp"

#include "GLCall.hpp"

VertexBuffer::VertexBuffer(void const* data, unsigned int size_)
	: mSize(size_)
{
	// Create the buffer
	GLCall(glGenBuffers(1, &mRendererID));

	// Fill the buffer
	bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size_, data, GL_STATIC_DRAW)); // The draw mode might become an option eventually
}

VertexBuffer::~VertexBuffer()
{
	// Destroy the buffer
	GLCall(glDeleteBuffers(1, &mRendererID));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}
void VertexBuffer::unbind() const
{
#ifndef RELEASE_MODE // No unecesary bind in release, but practical for debugging
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
#endif
}

void VertexBuffer::set(void const* data, unsigned int size_)
{
	if(size_ == mSize) {
		GLCall(glBufferData(GL_ARRAY_BUFFER, size_, data, GL_STATIC_DRAW)); // The draw mode might become an option eventually
	} else {
		LOG_WARNING("Cannot dynamically reallocate buffers yet...");
	}
}
