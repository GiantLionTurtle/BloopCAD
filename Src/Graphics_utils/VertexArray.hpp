
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

#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

/*
	@class VertexArray describes an abstraction of openGL's vertex arrays
	@warning : Make sure that every function is called whithin the same openGL context
*/
class VertexArray {
private:
	unsigned int mRendererID; // The openGL handle to the vertex array
public:
	/*
		@function VertexArray creates a VertexArray object
	*/
	VertexArray();
	/*
		@function ~VertexArray destroys a VertexArray object and cleanly handle the internal openGL stuff
	*/
	~VertexArray();

	/*
		@function add buffer adds a set of vertices to the vertex array

		@param vb : 	A VertexBuffer containing vertices data
		@param layout : A VertexBufferLayout describing the data of the VertexBuffer
	*/
	void add_buffer(VertexBuffer const& vb, VertexBufferLayout const& layout);

	/*
		@function bind activates the VertexArray in openGL so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the VertexArray in openGL so that it can't be used
	*/
	void unbind() const;

	/*
		@function id

		@return : The openGL handle to the vertex array
	*/
	unsigned int id() const { return mRendererID; }
};

#endif
