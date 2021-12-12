
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
    This shader draws a texture with a background color on the geometry
*/

#version 330

out vec4 fragColor;

uniform vec3 u_Color;

in data
{
	vec2 UV_coord;
}gs_out;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, gs_out.UV_coord);
    fragColor.xyz += fragColor.w * u_Color;
}
