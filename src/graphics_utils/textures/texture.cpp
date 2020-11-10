
#include "texture.hpp"

#include <graphics_utils/GLCall.hpp>

#include <stb_image.h>

texture::texture() :
	mRendererID(0),
	mLocalBuffer(nullptr),
	mWidth(0),
	mHeight(0),
	mBPP(0)
{}
texture::texture(int width, int height, unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int mag_filter, unsigned int channels):
	mLocalBuffer(nullptr),
	mWidth(width),
	mHeight(height)
{
	GLCall(glGenTextures( 1, &mRendererID ));
	bind();

	if(channels != 4) {
		LOG_ERROR("Only 4 channels is supported for the momemnt.");
	}

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
}
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
