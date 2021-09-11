
/*
	This shader draws a singular point of the specified radius on the geometry (usualy a quad)
*/

#version 330

out vec4 fragColor;

uniform vec4 u_Color;
uniform float u_Diameter;
uniform vec2 u_Viewport;

in fData
{
	float radius_square;
    vec2 center;
}frag;    

void main()
{	
	float d_x = (frag.center.x - gl_FragCoord.x);
	float d_y = (frag.center.y - gl_FragCoord.y);
	if((d_x*d_x + d_y*d_y) < frag.radius_square)
		fragColor = u_Color;
	else 
		fragColor = vec4(0.0, 1.0, 0.0, 0.0);//pow((1.0-len)/(feather), 2.5));
}
