
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

#include "Texture.hpp"

#include <string>

/*
	@class image describes a texture loaded from an image file
*/
class Image : public Texture {
private:
	std::string mFilePath; // The path to the image file
public:
	/*
		@function image creates an image object

		@param filePath The path to the file that contains the image
	*/
	Image(std::string const& filePath);
	/*
		@function ~image destroys the image texture and cleanly deallocate in openGL
	*/
	~Image();
};