
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

#ifndef VERTEXBUFFERLAYOUT_HPP_
#define VERTEXBUFFERLAYOUT_HPP_

#include <vector>

/*
	@struct VertexBufferProprety describes a single proprety of a vertex (position, color, etc..)
*/
struct VertexBufferProprety {
	unsigned int count; // Number of element in the proprety
	unsigned int type; // Type of the elements (float, int, etc..)
	unsigned int normalized; // Whether ot not the proprety is normalized

	/*
		@function typeSize

		@return : The size in bytes of the type of the proprety
	*/
	unsigned int typeSize() const;
};

/*
	@class VertexBufferLayout describes the way propreties are laid out in a vertex buffer
*/
class VertexBufferLayout {
private:
	std::vector<VertexBufferProprety> mPropreties; // All the propreties of the layout
	unsigned int mStride; // The total size in bytes of a vertex with all the propreties
public:
	/*
		@function VertexBufferLayout creates an empty layout
	*/
	VertexBufferLayout();

	/*
		@function add_proprety_float adds a proprety of type float to the layout

		@param count : The number of element in the proprety
	*/
	void add_proprety_float(unsigned int count);
	/*
		@function add_proprety_uint adds a proprety of type unsigned int to the layout

		@param count : The number of element in the proprety
	*/	
	void add_proprety_uint(unsigned int count);
	/*
		@function add_proprety_uchar adds a proprety of type unsigned char to the layout

		@param count : The number of element in the proprety
	*/	
	void add_proprety_uchar(unsigned int count);

	/*
		@function propreties 

		@return : All the propreties of the layout
	*/
	std::vector<VertexBufferProprety> const& propreties() const { return mPropreties; };
	/*
		@function stride 

		@return : The total size of a single vertex with all the propreties
	*/
	unsigned int stride() const { return mStride; }
};

#endif