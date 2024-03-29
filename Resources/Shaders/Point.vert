
/*
    This shader transforms 3d coordinate to screen coordinate
*/

#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition, 1.0);
}
