
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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