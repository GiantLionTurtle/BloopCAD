
#include "frameBuffer.hpp"

frameBuffer::frameBuffer(unsigned int width, unsigned int height):
	mTexture(width, height, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST, 4) // No need to get fancy with interpolation, it will likely be the exact same size as the screen
{
#ifdef BLOOP_TEST_SELCTIONCOLORS_ON_MAINBUFFER
#warning "Selection colors will be drawn on main buffer" // Do nothing to keeep it on the main frame buffer (screen)
#else
	int initialFrameBuffer; // Record the context's frame buffer
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &initialFrameBuffer);

	// Creation of frame buffer
	GLCall(glGenFramebuffers(1, &mRendererID));

	// Bind the frame buffer with mTexture
	bind();	
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.id(), 0));

	// Generate and setup render buffer (should the id be kept to dealocate it in the destructor??)
	unsigned int rb = 0;
	GLCall(glGenRenderbuffers(1, &rb));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rb));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));

	// Assign the render buffer to the frame buffer (and therefore the texture)
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb));
	
	// Set the output of the fragment shaders to our render buffer
	unsigned int draw_bufs[] = {GL_COLOR_ATTACHMENT0};
	GLCall(glDrawBuffers(1, draw_bufs));

	// Error check
	GLCall(unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		LOG_WARNING("Incomplete frame buffer: " + std::to_string(status));
	}
	
	// Clean up
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, initialFrameBuffer));
#endif
}
frameBuffer::~frameBuffer()
{
	// The destructor of mTexture should clean everything up
}

void frameBuffer::bind() const
{
#ifdef BLOOP_TEST_SELCTIONCOLORS_ON_MAINBUFFER
#warning "Selection colors will be drawn on main buffer" // Do nothing to keeep it on the main frame buffer (screen)
#else
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
#endif
}
void frameBuffer::unbind(unsigned int defaultID) const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, defaultID)); // Bind to an id (defaults is 0)
}