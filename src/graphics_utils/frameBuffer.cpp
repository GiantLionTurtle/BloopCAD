
#include "frameBuffer.hpp"

frameBuffer::frameBuffer(unsigned int width, unsigned int height):
	mTexture(width, height, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST, 4)
{
	int initialFrameBuffer;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &initialFrameBuffer);

	GLCall(glGenFramebuffers(1, &mRendererID));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.id(), 0));


	unsigned int rb = 0;
	GLCall(glGenRenderbuffers(1, &rb));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rb));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));

	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb));
	
	unsigned int draw_bufs[] = {GL_COLOR_ATTACHMENT0};
	GLCall(glDrawBuffers(1, draw_bufs));

	GLCall(unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		LOG_ERROR("Incomplete frame buffer: " + std::to_string(status));
	}
	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, initialFrameBuffer));
}
frameBuffer::~frameBuffer()
{
	
}

void frameBuffer::bind() const
{
#ifdef BLOOP_TEST_SELCTIONCOLORS_ON_MAINBUFFER
#warning "Selection colors will be drawn on main buffer"
#else
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
#endif
}
void frameBuffer::unbind(unsigned int defaultID) const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, defaultID));
}