
#include "indexBuffer.hpp"

#include "GLCall.hpp"

indexBuffer::indexBuffer(unsigned int const* data, unsigned int count_):
  mCount(count_)
{
  // Generate the buffer
  GLCall(glGenBuffers(1, &mRendererID));

  // Setup the data, maybe the draw type will be an option in the future
  bind();
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW)); 
}

indexBuffer::~indexBuffer()
{
  GLCall(glDeleteBuffers(1, &mRendererID)); // Delete the buffer
}

void indexBuffer::bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
}
void indexBuffer::unbind() const
{
#ifndef RELEASE_MODE // In release there is no unecesary binds, but it's cleaner for debugging
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
#endif
}
