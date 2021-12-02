
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

#ifndef GLCALL_HPP_
#define GLCALL_HPP_

#include <Utils/Debug_util.hpp>

#include <epoxy/gl.h>

/*
	@function GLClearErrors makes sure openGL has no error accumulated, since it can only give the newest one
*/
void GLClearErrors();
/*
	@function GLLogCall logs a warning if an internal error is detected in openGL
*/
bool GLLogCall(std::string const& call, std::string const& file, std::string const& function, unsigned int line);

/*
	@macro GLCall calls an openGL function and logs a warning if a new error appears
*/
#define GLCall(x) GLClearErrors();\
	x;\
	GLLogCall(#x, __FILE__, __FUNCTION__, __LINE__)

// #define LOG_EVERY_GLCALL // Uncomment to have every openGL call logged, error or no error

#endif