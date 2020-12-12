
#include "vertexBufferLayout.hpp"
#include "GLCall.hpp"

vertexBufferLayout::vertexBufferLayout():
  mStride(0)
{

}

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
  mPropreties.push_back({ count, GL_FLOAT, GL_FALSE }); // Normalized is false, not an option
  mStride += sizeof(float) * count; // The stride expands with each addition of proprety
}
void vertexBufferLayout::add_proprety_uint(unsigned int count)
{
  mPropreties.push_back({ count, GL_UNSIGNED_INT, GL_FALSE }); // Normalized is false, not an option
  mStride += sizeof(unsigned int) * count; // The stride expands with each addition of proprety
}
void vertexBufferLayout::add_proprety_uchar(unsigned int count)
{
  mPropreties.push_back({ count, GL_UNSIGNED_BYTE, GL_FALSE }); // Normalized is false, not an option
  mStride += sizeof(unsigned char) * count; // The stride expands with each addition of proprety
}
