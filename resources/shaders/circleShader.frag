#version 330

out vec4 fragColor;

uniform vec4 u_Color;
uniform vec2 u_Center;
uniform float u_R2;

void main()
{
	float d_x = (u_Center.x - gl_FragCoord.x);
	float d_y = (u_Center.y - gl_FragCoord.y);
	float d = (d_x*d_x + d_y*d_y);
	if(d < u_R2 && d > (u_R2 - 800))
		fragColor = u_Color;
	else 
		fragColor = vec4(0.0, 1.0, 0.0, 0.0);
}
