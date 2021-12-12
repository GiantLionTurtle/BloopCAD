
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
	This shader draws a singular point of the specified radius on the geometry (usualy a quad)
*/

#version 330

out vec4 fragColor;

uniform vec4 u_Color;
uniform float u_Diameter;
uniform vec2 u_Viewport;

in fData
{
	float radius_square;
    vec2 center;
}frag;    

void main()
{	
	float d_x = (frag.center.x - gl_FragCoord.x);
	float d_y = (frag.center.y - gl_FragCoord.y);
	if((d_x*d_x + d_y*d_y) < frag.radius_square)
		fragColor = u_Color;
	else 
		fragColor = vec4(0.0, 1.0, 0.0, 0.0);//pow((1.0-len)/(feather), 2.5));
}
