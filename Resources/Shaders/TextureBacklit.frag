
/*
    This shader draws a texture with a background color on the geometry
*/

#version 330

out vec4 fragColor;

uniform vec3 u_Color;

in data
{
	vec2 UV_coord;
}gs_out;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, gs_out.UV_coord);
    fragColor.xyz += fragColor.w * u_Color;
}
