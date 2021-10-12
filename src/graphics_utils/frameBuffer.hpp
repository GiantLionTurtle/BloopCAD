
#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "textures/texture.hpp"

#include "GLCall.hpp"

// #define BLOOP_TEST_SELCTIONCOLORS_ON_MAINBUFFER // Used to debug the selection buffer by rendering to the screen

/*
    @class frameBuffer describes an abstraction of a GL_FRAMEBUFFER specifically to use it as a selection buffer

    @warning : Make sure that all function are used within the same openGL context
*/
class frameBuffer {
private:
    unsigned int mRendererID; // The generated openGL id    
    texture mTexture; // The texture it represents... maybe it should just derive from texture instead
public:
    /*
        @function frameBuffer creates a frameBuffer object

        @param width :  The width in pixels of the frameBuffer
        @param height : The height in pixels of the frameBuffer
    */
    frameBuffer(unsigned int width, unsigned int height);
    /*
        @function ~frameBuffer destroys the frameBuffer and cleans up openGL handles
    */
    ~frameBuffer();

    /*
        @function bind activates the frameBuffer within openGL
    */
    void bind() const;
    /*
        @function unbind desactivate the frameBuffer within openGL

        @param defaultID [defaults to 0] : A GL_FRAMEBUFFER id to bind instead
    */
    void unbind(unsigned int defaultID = 0) const;

    /*
        @function id

        @return : The openGL id of the frameBuffer
    */
    unsigned int id() const { return mRendererID; }
};

#endif