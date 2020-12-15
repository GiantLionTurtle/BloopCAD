
#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 10) out;

uniform vec2 u_Viewport;
uniform float u_LineWidth;

void main() 
{
	vec2 aspectVec = vec2(u_Viewport.x/u_Viewport.y, 1.0);
	vec2 pos_screen0 = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w * aspectVec;
	vec2 pos_screen1 = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w * aspectVec;

	vec2 direction = normalize(pos_screen1 - pos_screen0);
	vec2 normal = (vec2(-direction.y, direction.x) / u_Viewport) * u_LineWidth;

	gl_Position = gl_in[0].gl_Position + vec4(normal, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position - vec4(normal, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position + vec4(normal, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[1].gl_Position - vec4(normal, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  