
#include "vertexBufferLayout.hpp"
#include "GLCall.hpp"

unsigned int vertexBufferProprety::typeSize() const
{
  switch(type) {
    case GL_FLOAT:        return 4;
    case GL_UNSIGNED_INT: return 4;
    case GL_UNSIGNED_BYTE:return 1;
  };

  LOG_WARNING("Unknown type.");
  return 0;
}

void vertexBufferLayout::add_proprety_float(unsigned int count)
{
  mPropreties.push_back({ count, GL_FLOAT, GL_FALSE });
  mStride += sizeof(float) * count;
}
void vertexBufferLayout::add_proprety_uint(unsigned int count)
{
  mPropreties.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
  mStride += sizeof(unsigned int) * count;
}
void vertexBufferLayout::add_proprety_uchar(unsigned int count)
{
  mPropreties.push_back({ count, GL_UNSIGNED_BYTE, GL_FALSE });
  mStride += sizeof(unsigned char) * count;
}
