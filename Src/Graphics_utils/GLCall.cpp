
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

#include "GLCall.hpp"

void GLClearErrors()
{
  while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(std::string const& call, std::string const& file, std::string const& function, unsigned int line)
{
	if(GLenum error = glGetError()) {
		ErrorLogger::get_instance().log("OpenGL error", file, function, line, "while calling " + call + ": " + std::to_string(error));
		return false;
	} else {
#ifdef LOG_EVERY_GLCALL
		ErrorLogger::get_instance().log("Called", file, function, line, call);
#endif
	}
	return true;
}