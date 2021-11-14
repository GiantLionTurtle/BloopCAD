
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

#include "IndexBuffer.hpp"

#include "GLCall.hpp"

IndexBuffer::IndexBuffer(unsigned int const* data, unsigned int count_)
	: mCount(count_)
{
	// Generate the buffer
	GLCall(glGenBuffers(1, &mRendererID));

	// Setup the data, maybe the draw type will be an option in the future
	bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW)); 
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &mRendererID)); // Delete the buffer
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
}
void IndexBuffer::unbind() const
{
#ifndef RELEASE_MODE // In release there is no unecesary binds, but it's cleaner for debugging
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
#endif
}
