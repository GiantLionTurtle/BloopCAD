
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
	This shader expands a point into a quad of specified width and height
*/

#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 u_Viewport;
uniform float u_Width, u_Height;

out data
{
	vec2 UV_coord;
}gs_out;

void main() 
{
	// Find the half width and half height of the quad in normalized coordinates
	float mv_x = gl_in[0].gl_Position.w * (u_Width / u_Viewport.x) / 2;
	float mv_y = gl_in[0].gl_Position.w * (u_Height / u_Viewport.y) / 2;

	gs_out.UV_coord = vec2(1.0, 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(mv_x, mv_y, 0.0, 0.0);
	EmitVertex();
	gs_out.UV_coord = vec2(0.0, 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(-mv_x, mv_y, 0.0, 0.0);
	EmitVertex();
	gs_out.UV_coord = vec2(1.0, 0.0);
	gl_Position = gl_in[0].gl_Position + vec4(mv_x, -mv_y, 0.0, 0.0);
	EmitVertex();
	gs_out.UV_coord = vec2(0.0, 0.0);
	gl_Position = gl_in[0].gl_Position + vec4(-mv_x, -mv_y, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  