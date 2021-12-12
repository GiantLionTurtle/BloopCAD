
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

/*
    This shader transforms 3d coordinates into screen coordinates and passes along texture data
*/

#version 330

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;

out vec2 v_TextureCoordinates;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition.xy, 0.0, 1.0);
    v_TextureCoordinates = textureCoordinates;
}
