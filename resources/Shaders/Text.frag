
/*
    This shader draws a text texture
*/

#version 330

out vec4 fragColor;

in vec2 v_TextureCoordinates;

uniform sampler2D u_Texture;
uniform vec3 u_Color;

void main()
{
    vec4 tex_s = texture(u_Texture, v_TextureCoordinates);
    fragColor = vec4(u_Color, tex_s.r);
}
