
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 u_Viewport;
uniform vec2 u_PixelOffset;
uniform float u_Width, u_Height;

out data
{
	vec2 UV_coord;
}gs_out;

void main() 
{
	vec2 cent = (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w + vec2(1, 1)) / 2 * u_Viewport;
	
	float mv_x = gl_in[0].gl_Position.w * (u_Width / u_Viewport.x) / 2;
	float mv_y = gl_in[0].gl_Position.w * (u_Height / u_Viewport.y) / 2;

	vec2 offset2 = gl_in[0].gl_Position.w * u_PixelOffset / u_Viewport * 2;
	vec4 offset = vec4(offset2, 0, 0);

	gs_out.UV_coord = vec2(1.0, 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(mv_x, mv_y, 0.0, 0.0) + offset;
	EmitVertex();
	gs_out.UV_coord = vec2(0.0, 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(-mv_x, mv_y, 0.0, 0.0) + offset;
	EmitVertex();
	gs_out.UV_coord = vec2(1.0, 0.0);
	gl_Position = gl_in[0].gl_Position + vec4(mv_x, -mv_y, 0.0, 0.0) + offset;
	EmitVertex();
	gs_out.UV_coord = vec2(0.0, 0.0);
	gl_Position = gl_in[0].gl_Position + vec4(-mv_x, -mv_y, 0.0, 0.0) + offset;
	EmitVertex();

	EndPrimitive();
}  