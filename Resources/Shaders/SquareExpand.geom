
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