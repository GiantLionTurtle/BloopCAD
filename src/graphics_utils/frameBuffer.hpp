
#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "GLCall.hpp"

#define BLOOP_TEST_SELCTIONCOLORS_ON_MAINBUFFER

class frameBuffer {
private:
    unsigned int mRendererID;
public:
    frameBuffer();
    ~frameBuffer();

    void bind() const;
    void unbind() const;
};

#endif