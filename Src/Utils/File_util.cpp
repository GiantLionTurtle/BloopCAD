
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

#include "File_util.hpp"
#include <Utils/Debug_util.hpp>

#include <fstream>
#include <sstream>

std::string readFromFile(std::string const& filePath)
{
	std::ifstream stream(filePath);
	if(!stream) {
		LOG_WARNING("Could not open file \"" + filePath + "\"");
		return "";
	}

	std::string str(std::istreambuf_iterator<char>(stream), {}); // Some stl wizardry
	return str;
}