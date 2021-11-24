
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

#include "GLCall.hpp"
#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	// Create the vertex array
	GLCall(glGenVertexArrays(1, &mRendererID));
}
VertexArray::~VertexArray()
{
	// Delete the vertex array
	GLCall(glDeleteVertexArrays(1, &mRendererID));
}

void VertexArray::add_buffer(VertexBuffer const& vb, VertexBufferLayout const& layout)
{
	vb.bind(); // Bind the vertex buffer to add it to the vertex array

	// Set the desired layout for the buffer within the vertex array
	std::vector<VertexBufferProprety> const& propreties = layout.propreties();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < propreties.size(); ++i) {
		VertexBufferProprety const& proprety = propreties[i]; // Current proprety

		GLCall(glEnableVertexAttribArray(i)); // Enable said proprety
		GLCall(glVertexAttribPointer(i, proprety.count, proprety.type, proprety.normalized, layout.stride(), (void*)(intptr_t)offset)); // Describe proprety
		offset += proprety.count * proprety.typeSize(); // Increase our position in the vertex array's index
	}

	// Clean up
	vb.unbind();
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(mRendererID));
}
void VertexArray::unbind() const
{
#ifndef RELEASE_MODE // No unecesary binding in release, but practical for debugging purposes
	GLCall(glBindVertexArray(0));
#endif
}
