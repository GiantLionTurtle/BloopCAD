#version 330

out vec4 fragColor;

uniform vec3 u_Color;

void main()
{
    fragColor = vec4(u_Color.xyz, 1.0);
}
