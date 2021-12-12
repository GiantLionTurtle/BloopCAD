
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
	This shader expands a point into a square of specified side
*/

#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 u_Viewport;
uniform float u_Diameter;

out fData
{
	float radius_square;
    vec2 center;
}frag;    

void main() 
{
	vec2 cent = (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w + vec2(1, 1)) / 2 * u_Viewport;
	
	float mv_x = gl_in[0].gl_Position.w * (u_Diameter / u_Viewport.x);
	float mv_y = gl_in[0].gl_Position.w * (u_Diameter / u_Viewport.y);
	float rad_sq = pow(u_Diameter/2.0, 2.0);

	frag.radius_square = rad_sq;
	frag.center = cent;
	gl_Position = gl_in[0].gl_Position + vec4(mv_x, mv_y, 0.0, 0.0);
	EmitVertex();
	frag.radius_square = rad_sq;
	frag.center = cent;
	gl_Position = gl_in[0].gl_Position + vec4(-mv_x, mv_y, 0.0, 0.0);
	EmitVertex();
	frag.radius_square = rad_sq;
	frag.center = cent;
	gl_Position = gl_in[0].gl_Position + vec4(mv_x, -mv_y, 0.0, 0.0);
	EmitVertex();
	frag.radius_square = rad_sq;
	frag.center = cent;
	gl_Position = gl_in[0].gl_Position + vec4(-mv_x, -mv_y, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  