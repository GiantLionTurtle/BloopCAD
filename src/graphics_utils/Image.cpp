
#include "Image.hpp"
#include <graphics_utils/GLCall.hpp>

#include <stb_image.h>

Image::Image(std::string const& filePath):
	mFilePath(filePath)
{
	// Load the image using stbi_image
	stbi_set_flip_vertically_on_load(true);
	mLocalBuffer = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mBPP, 4);

	// Parametrize the texture
	bind();
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Fill the texture with the image data
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));

	// Clean up
	if(mLocalBuffer)
		stbi_Image_free(mLocalBuffer);
	unbind();
}
Image::~Image()
{

}