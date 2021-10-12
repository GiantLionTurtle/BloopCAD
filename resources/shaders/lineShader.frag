#version 330

out vec4 fragColor;

uniform vec4 u_Color;
uniform float u_Feather;

in fData
{
    vec2 normal;
}frag;

void main()
{
	float len = length(frag.normal);
	float feather = u_Feather;
	if(len < (1-feather))
		fragColor = u_Color;
	else 
		fragColor = vec4(u_Color.xyz, pow((1.0-len)/(feather), 2.5));
}
