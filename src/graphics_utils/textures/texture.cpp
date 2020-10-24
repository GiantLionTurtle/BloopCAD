
#include "texture.hpp"

#include "../GLCall.hpp"

#include <stb_image.h>

texture::texture() :
  mRendererID(0),
  mLocalBuffer(nullptr),
  mWidth(0),
  mHeight(0),
  mBPP(0)
{}
texture::~texture()
{
  GLCall(glDeleteTextures(1, &mRendererID));
}

void texture::bind(unsigned int slot/* = 0*/) const
{
  GLCall(glActiveTexture(GL_TEXTURE0+slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
}
void texture::unbind() const
{
#ifndef RELEASE_MODE // legit in this context?
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
#endif
}
