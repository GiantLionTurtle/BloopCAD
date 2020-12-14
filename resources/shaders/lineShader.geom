// #version 330

// layout (lines) in;
// layout (triangle_strip, max_vertices = 4) out;
 
// uniform float u_LineWidth;
// uniform mat4 u_MVP;
// uniform vec2 u_Viewport;

// bool is_visible(vec4 pt) 
// {
// 	return abs(pt.x) < pt.w && abs(pt.y) < pt.w && abs(pt.z) < pt.w;
// }

// void main()
// {
// 	vec4 NDC_prev 	= gl_in[0].gl_Position;
// 	vec4 NDC_start 	= gl_in[1].gl_Position;
// 	vec4 NDC_end 	= gl_in[2].gl_Position;
// 	vec4 NDC_next 	= gl_in[3].gl_Position;

// 	vec2 screen_prev 	= u_Viewport * ((NDC_prev.xy	/ NDC_prev.w) 	+ 1) / 2.0;
// 	vec2 screen_start 	= u_Viewport * ((NDC_start.xy	/ NDC_start.w) 	+ 1) / 2.0;
// 	vec2 screen_end 	= u_Viewport * ((NDC_end.xy		/ NDC_end.w) 	+ 1) / 2.0;
// 	vec2 screen_next 	= u_Viewport * ((NDC_next.xy	/ NDC_next.w) 	+ 1) / 2.0;

// 	vec2 dir = normalize(screen_end - screen_start);
// 	vec2 norm = vec2(-dir.y, dir.x);
// 	norm *= u_LineWidth / 2.0;

// 	gl_Position = vec4(2.0 * (screen_start + norm) / u_Viewport - 1, NDC_start.z / NDC_start.w, 1.0);
// 	EmitVertex();
// 	gl_Position = vec4(2.0 * (screen_start - norm) / u_Viewport - 1, NDC_start.z / NDC_start.w, 1.0);
// 	EmitVertex();
// 	gl_Position = vec4(2.0 * (screen_end + norm) / u_Viewport - 1, NDC_start.z / NDC_start.w, 1.0);
// 	EmitVertex();
// 	gl_Position = vec4(2.0 * (screen_end - norm) / u_Viewport - 1, NDC_start.z / NDC_start.w, 1.0);
// 	EmitVertex();

// 	EndPrimitive();
// }

#version 330 core
layout (lines) in;
//layout (triangle_strip, max_vertices = 10) out;
layout (lines) out;

uniform float u_AspectRatio;

void main() 
{
	/*
	vec2 direction = normalize(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec2 normal = vec2(-direction.y, direction.x * u_AspectRatio);

	gl_Position = gl_in[0].gl_Position + vec4(normal * 0.2, 0, 0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position - vec4(normal * 0.2, 0, 0);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position + vec4(normal * 0.2, 0, 0);
	EmitVertex();
	gl_Position = gl_in[1].gl_Position - vec4(normal * 0.2, 0, 0);
	EmitVertex();

	EndPrimitive();
	*/
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
}  