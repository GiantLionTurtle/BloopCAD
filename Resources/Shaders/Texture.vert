
/*
    This shader transforms 3d coordinates into screen coordinates and passes along texture data
*/

#version 330

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;

out vec2 v_TextureCoordinates;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(vertexPosition.xy, 0.0, 1.0);
    v_TextureCoordinates = textureCoordinates;
}
