
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
    This shader draws a text texture
*/

#version 330

out vec4 fragColor;

in vec2 v_TextureCoordinates;

uniform sampler2D u_Texture;
uniform vec3 u_Color;

void main()
{
    vec4 tex_s = texture(u_Texture, v_TextureCoordinates);
    fragColor = vec4(u_Color, tex_s.r);
}
