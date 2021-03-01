#version 330

out vec4 fragColor;

in data
{
	vec2 UV_coord;
}gs_out;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, gs_out.UV_coord);
}
