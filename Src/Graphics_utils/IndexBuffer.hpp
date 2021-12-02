
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

#ifndef INDEXBUFFER_HPP_
#define INDEXBUFFER_HPP_

/*
	@class IndexBuffer describes an abstraction of an openGL indices buffer
	@warning Make sure all the functions are called within the same openGL context
*/
class IndexBuffer {
private:
	unsigned int mRendererID; // The internal openGL id
	unsigned int mCount; // The NUMBER of indices contained
public:
	/*
		@function IndexBuffer creates an IndexBuffer object

		@param data 	A pointer to the data assigned to the buffer, currently, only unsigned ints are supported
		@param count_ 	The NUMBER of data points
	*/
	IndexBuffer(unsigned int const* data, unsigned int count_);
	/*
		@function ~IndexBuffer destroys the IndexBuffer object and cleans it's openGL handle
	*/
	~IndexBuffer();

	/*
		@function bind activates the IndexBuffer inside openGL
	*/
	void bind() const;
	void unbind() const;
	/*
		@function unbind desactivates the IndexBuffer inside openGL
	*/

	/*
		@function count 

		@return The NUMBER of indices in the buffer
	*/
	unsigned int count() const { return mCount; };
	/*
		@function id

		@return The internal openGL handle to the buffer
	*/
	unsigned int id() const { return mRendererID; }
};

#endif
