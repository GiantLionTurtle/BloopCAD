
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

#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

/*
	@class VertexBuffer describes an abstraction of openGL's vertex buffer
	@warning : Make sure that all functions are called whithin the same openGL context
*/
class VertexBuffer {
private:
	unsigned int mRendererID; // The openGL handle to the buffer
	unsigned int mSize; // The size in bytes of the buffer
public:
	/*
		@function VertexBuffer creates a VertexBuffer object

		@param data : 	The data of the VertexBuffer, it can be of any types
		@param size_ : 	The size in bytes of the buffer
	*/
	VertexBuffer(void const* data, unsigned int size_);
	VertexBuffer(unsigned int size_);
	/*
		@function ~VertexBuffer destroys a VertexBuffer object and cleanly handles the deallocation in openGL
	*/
	~VertexBuffer();

	/*
		@function bind activates the buffer in openGL so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the buffer in openGL so that it can't be used anymore
	*/
	void unbind() const;

	/*
		@function size

		@return : The size in bytes of the buffer
	*/
	unsigned int size() const { return mSize; }
	/*
		@function id

		@return : The openGL handle of the buffer
	*/
	unsigned int id() const { return mRendererID; }

	void set(void const* data, unsigned int size_);
};

#endif
