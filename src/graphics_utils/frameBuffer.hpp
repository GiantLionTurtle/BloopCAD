
#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "textures/texture.hpp"

#include "GLCall.hpp"

//#define BLOOP_TEST_SELCTIONCOLORS_ON_MAINBUFFER

class frameBuffer {
private:
    unsigned int mRendererID;
    
    texture mTexture;
public:
    frameBuffer(unsigned int width, unsigned int height);
    ~frameBuffer();

    void bind() const;
    void unbind(unsigned int defaultID = 0) const;
};

#endif