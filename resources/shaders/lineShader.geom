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
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

void build_house(vec4 position)
{    
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}  