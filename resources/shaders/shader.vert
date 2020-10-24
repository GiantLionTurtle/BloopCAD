#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;

out vec2 v_TextureCoordinates;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition.xyz, 1.0);
    v_TextureCoordinates = textureCoordinates;
}
