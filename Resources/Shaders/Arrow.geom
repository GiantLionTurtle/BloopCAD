
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

in VS_OUT {
    vec3 color;
} gs_in[];  

void main() 
{
    /*
            *
           ***
          *****
        
        The arrow sprite looks something like this, the tip is in the midle
        at the top, it is the position that is in the input
    */

	vec2 dir = gs_in[0].xy / gs_in[0].w;
	vec2 mv_w = vec2(dir.y, -dir.x) * u_Width / u_Viewport / 2.0;
	vec2 mv_h = dir * u_Height / u_Viewport;

	gs_out.UV_coord = vec2(1.0, 0.0);
	gl_Position = gl_in[0].gl_Position + vec4(mv_w, 0.0, 0.0, 0.0);
	EmitVertex();
	gs_out.UV_coord = vec2(0.0, 0.0);
	gl_Position = gl_in[0].gl_Position + vec4(-mv_w, 0, 0.0, 0.0);
	EmitVertex();
	gs_out.UV_coord = vec2(1.0, 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(-mv_w, mv_h, 0.0, 0.0);
	EmitVertex();
	gs_out.UV_coord = vec2(1.0, 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(mv_w, mv_h, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  