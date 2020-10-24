#version 330

out vec4 fragColor;

in vec2 v_TextureCoordinates;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, v_TextureCoordinates);
}
