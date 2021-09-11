
/*
    This shader is a normal point shader that also passes along useful circle data
*/

#version 330

layout(location = 0) in vec3 center;
layout(location = 1) in float radius;
layout(location = 2) in vec3 v;
layout(location = 3) in vec3 w;

uniform mat4 u_MVP;

out vs_out {
    float radius;   // Radius of the circle
    vec4 v;         // One of the axis of the plane (normalized for accurate size representation)
    vec4 w;         // One of the axis of the plane (normalized for accurate size representation)
} vrtOut;


void main()
{
    gl_Position = u_MVP * vec4(center, 1.0);

    vrtOut.radius = radius;
    vrtOut.v = u_MVP * vec4(v, 0.0);
    vrtOut.w = u_MVP * vec4(w, 0.0);
}
