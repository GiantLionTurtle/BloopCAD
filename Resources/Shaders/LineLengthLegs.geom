
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

#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 u_Viewport;
uniform float u_LineWidth;
uniform float u_Feather;

out fData
{
    vec2 normal;
}frag;

void main() 
{
	vec2 aspectVec = vec2(u_Viewport.x/u_Viewport.y, 1.0);
	vec2 pos_screen0 = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w * aspectVec;
	vec2 pos_screen1 = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w * aspectVec;

	vec2 direction = normalize(pos_screen1 - pos_screen0);
    vec2 gap_offset = 20.0 * -direction / u_Viewport * gl_in[0].gl_Position.w;
	vec2 normal = (vec2(-direction.y, direction.x)) / u_Viewport * (u_LineWidth/2/(1-u_Feather)) * gl_in[0].gl_Position.w;

	vec2 norm_normal = normalize(normal);

	frag.normal = norm_normal;
	gl_Position = gl_in[0].gl_Position + vec4(normal, 0.0, 0.0) + vec4(gap_offset, 0.0, 0.0);
	EmitVertex();
	frag.normal = -norm_normal;
	gl_Position = gl_in[0].gl_Position - vec4(normal, 0.0, 0.0) + vec4(gap_offset, 0.0, 0.0);;
	EmitVertex();

	frag.normal = norm_normal;
	gl_Position = gl_in[1].gl_Position + vec4(normal, 0.0, 0.0) + vec4(gap_offset, 0.0, 0.0);;
	EmitVertex();
	frag.normal = -norm_normal;
	gl_Position = gl_in[1].gl_Position - vec4(normal, 0.0, 0.0) + vec4(gap_offset, 0.0, 0.0);;
	EmitVertex();

	EndPrimitive();
}  