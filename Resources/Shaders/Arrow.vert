
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

#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 arrowDirections;

uniform mat4 u_MVP;
uniform vec2 u_Viewport;

out VS_OUT {
    vec4 dir;
} vs_out;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition.xyz, 1.0);
    vs_out.dir  = normalize(u_MVP * vec4(arrowDirections.xyz, 0.0));
}
