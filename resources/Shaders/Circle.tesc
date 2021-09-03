
// Shader adapted from https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/tessellation.1pp.pdf

/*
    This shader determines the tesselation level of a circle
*/

#version 400

uniform float u_Scale;
layout(vertices = 1)  out;

in vs_out {
    float radius;
    vec4 v;
    vec4 w;
} tscIn[];

out tsc_out {
    float radius;
    vec4 v;
    vec4 w;
} tscOut[];


void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    
    // Pass through infos from vertex shader, might be redundant..?
    tscOut[gl_InvocationID].radius = tscIn[gl_InvocationID].radius;
    tscOut[gl_InvocationID].v = tscIn[gl_InvocationID].v;
    tscOut[gl_InvocationID].w = tscIn[gl_InvocationID].w;

    // Set tesselation level
    gl_TessLevelOuter[0] = 1.0; // Must be 1 because it renders a single curve
    gl_TessLevelOuter[1] = 10 + pow(10, 5 * u_Scale) + pow(10, 5 * tscIn[gl_InvocationID].radius); // TODO: calculate this instead of having it a fixed value
}