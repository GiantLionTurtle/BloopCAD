
#include "indexBuffer.hpp"

#include "GLCall.hpp"

indexBuffer::indexBuffer(unsigned int const* data, unsigned int count_):
  mCount(count_)
{
  GLCall(glGenBuffers(1, &mRendererID));
  bind();
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

indexBuffer::~indexBuffer()
{
  GLCall(glDeleteBuffers(1, &mRendererID));
}

void indexBuffer::bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
}
void indexBuffer::unbind() const
{
#ifndef RELEASE_MODE
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
#endif
}
