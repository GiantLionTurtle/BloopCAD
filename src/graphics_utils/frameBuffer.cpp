
#include "frameBuffer.hpp"

frameBuffer::frameBuffer()
{
    GLCall(glGenFramebuffers(1, &mRendererID));

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
void frameBuffer::unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}