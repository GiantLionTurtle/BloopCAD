
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
	This shader draws an antialiased line segment 
*/

#version 330

out vec4 fragColor;

uniform vec4 u_Color;
uniform float u_Feather;

in fData
{
    vec2 normal;
}frag;

void main()
{
	float len = length(frag.normal);
	float feather = u_Feather;
	if(len < (1-feather))
		fragColor = u_Color;
	else 
		fragColor = vec4(u_Color.xyz, pow((1.0-len)/(feather), 2.5));
}
