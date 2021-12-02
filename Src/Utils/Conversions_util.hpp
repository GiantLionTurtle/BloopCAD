
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

#ifndef CONVERSIONSUTILS_HPP_
#define CONVERSIONSUTILS_HPP_

#include <glm/glm.hpp>

#include <string>

/*
    @function string_to_float attempts to create a float from a string

    @return : The created float or 0.0f
*/
float string_to_float(std::string const& str);
/*
    @function string_to_long attempts to create a long from a string

    @return : The created long or 0
*/
long string_to_long(std::string const& str);
/*
    @function string_to_vec3 attempts to create a vec3 from a string

    @return : The created vec3 or (0.0f, 0.0f, 0.0f)
*/
glm::vec3 string_to_vec3(std::string const& str);

#endif