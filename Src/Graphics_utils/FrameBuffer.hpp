
#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "Texture.hpp"

/*
    @class FrameBuffer describes an abstraction of a GL_FRAMEBUFFER specifically to use it as a selection buffer

    @warning Make sure that all function are used within the same openGL context
*/
class FrameBuffer {
private:
    unsigned int mRendererID; // The generated openGL id    
    Texture mTexture; // The texture it represents... maybe it should just derive from texture instead
public:
    /*
        @function FrameBuffer creates a FrameBuffer object

        @param width  The width in pixels of the FrameBuffer
        @param height The height in pixels of the FrameBuffer
    */
    FrameBuffer(unsigned int width, unsigned int height);
    /*
        @function ~FrameBuffer destroys the FrameBuffer and cleans up openGL handles
    */
    ~FrameBuffer();

    /*
        @function bind activates the FrameBuffer within openGL
    */
    void bind() const;
    /*
        @function unbind desactivate the FrameBuffer within openGL

        @param defaultID [defaults to 0] A GL_FRAMEBUFFER id to bind instead
    */
    void unbind(unsigned int defaultID = 0) const;

    /*
        @function id

        @return The openGL id of the FrameBuffer
    */
    unsigned int id() const { return mRendererID; }
};

#endif