
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Texture.hpp"

#include <Graphics_utils/GLCall.hpp>

#include <stb_image.h>

Texture::Texture()
	: mRendererID(0)
	, mLocalBuffer(nullptr)
	, mWidth(0)
	, mHeight(0)
	, mBPP(0)
{
	GLCall(glGenTextures(1, &mRendererID));
}
Texture::Texture(int width, int height, unsigned int wrap_s, unsigned int wrap_t, 
				unsigned int min_filter, unsigned int mag_filter, unsigned int channels)
	: mLocalBuffer(nullptr)
	, mWidth(width)
	, mHeight(height)
{
	// Create the texture
	GLCall(glGenTextures( 1, &mRendererID ));
	
	if(channels != 4) {
		LOG_WARNING("Only 4 channels is supported for the momemnt.");
	}

	// Parametrize the texture
	bind();
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));

	// Clean up
	unbind();
}
Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mRendererID));
}

void Texture::bind(unsigned int slot/* = 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0+slot)); // All Texture slots ids are contiguous 
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
}
void Texture::unbind() const
{
#ifndef RELEASE_MODE // legit in this context?
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
#endif
}
