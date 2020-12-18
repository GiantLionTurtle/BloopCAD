
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 u_Viewport;
uniform float u_LineWidth;
// uniform float u_Feather;

// out fData
// {
//     vec2 normal;
// }frag;    

void main() 
{
	vec2 aspectVec = vec2(u_Viewport.x/u_Viewport.y, 1.0);
	// vec2 pos_screen0 = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w * aspectVec;
	// vec2 pos_screen1 = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w * aspectVec;

	gl_Position = gl_in[0].gl_Position + glm::vec4(0.1, 0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + glm::vec4(-0.1, 0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + glm::vec4(0.1, -0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + glm::vec4(-0.1, -0.1, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  