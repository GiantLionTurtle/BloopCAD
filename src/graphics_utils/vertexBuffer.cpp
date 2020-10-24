
#include "vertexBuffer.hpp"

#include "GLCall.hpp"

vertexBuffer::vertexBuffer(void const* data, unsigned int size_) :
mSize(size_)
{
  GLCall(glGenBuffers(1, &mRendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size_, data, GL_STATIC_DRAW));
}

vertexBuffer::~vertexBuffer()
{
  GLCall(glDeleteBuffers(1, &mRendererID));
}

void vertexBuffer::bind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}
void vertexBuffer::unbind() const
{
#ifndef RELEASE_MODE
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
#endif
}

unsigned int vertexBuffer::size()
{
  return mSize;
}
