#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(vertexPosition.xyz, 1.0);
}
