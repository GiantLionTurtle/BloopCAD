
#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 pixelOffset;

uniform mat4 u_MVP;
uniform vec2 u_Viewport;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition, 1.0);
    vec2 offset2 = gl_Position.w * pixelOffset / u_Viewport * 2;
	vec4 offset = vec4(offset2, 0, 0);
    gl_Position += offset;
}
