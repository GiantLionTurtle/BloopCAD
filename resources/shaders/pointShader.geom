
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 u_Viewport;
uniform float u_Radius;

out fData
{
    vec2 center;
}frag;    

void main() 
{
	vec2 aspectVec = vec2(u_Viewport.x/u_Viewport.y, 1.0);
	vec2 pos_screen = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w * aspectVec;
	
	float mv_x = gl_in[0].gl_Position.w * (u_Radius / u_Viewport.x);
	float mv_y = gl_in[0].gl_Position.w * (u_Radius / u_Viewport.y);

	frag.center = pos_screen;
	gl_Position = gl_in[0].gl_Position + glm::vec4(mv_x, mv_y, 0.0, 0.0);
	EmitVertex();
	frag.center = pos_screen;
	gl_Position = gl_in[0].gl_Position + glm::vec4(-mv_x, mv_y, 0.0, 0.0);
	EmitVertex();
	frag.center = pos_screen;
	gl_Position = gl_in[0].gl_Position + glm::vec4(mv_x, -mv_y, 0.0, 0.0);
	EmitVertex();
	frag.center = pos_screen;
	gl_Position = gl_in[0].gl_Position + glm::vec4(-mv_x, -mv_y, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  