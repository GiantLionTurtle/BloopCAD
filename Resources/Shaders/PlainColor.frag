
/*
    This shader draws a plain color on the geometry
*/

#version 330

out vec4 fragColor;

uniform vec4 u_Color;

void main()
{
    fragColor = vec4(u_Color);
}
