#version 330

out vec4 fragColor;

uniform vec4 u_Color;
uniform float u_Radius;

in fData
{
    vec2 center;
}frag;    

void main()
{	
	//float d_x = frag.center.x - gl_FragCoord.x;
	//float d_y = frag.center.y - gl_FragCoord.y;
	//if(sqrt(d_x*d_x + d_y*d_y) < u_Radius * u_Radius)
		fragColor = u_Color;
	//else 
	//	fragColor = vec4(u_Color.xyz, 0.0);//pow((1.0-len)/(feather), 2.5));
}
