
// Shader adapted from https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/tessellation.1pp.pdf

#version 400

layout(isolines, equal_spacing, ccw)  in;

uniform vec2 u_Viewport;

in tsc_out {
    float radius;
    vec4 v;
    vec4 w;
} tseIn[];


void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    float t = gl_TessCoord.x * 2 * 3.1415;
    vec3 aspectVec = vec3(u_Viewport.x/u_Viewport.y, 1.0, 1.0);
    vec4 v_vec = tseIn[0].v;
    vec4 w_vec = tseIn[0].w;
    float rad = tseIn[0].radius;

    // The equation for parametric circle in 3d space taken from:
    // https://math.stackexchange.com/questions/73237/parametric-equation-of-a-circle-in-3d-space
    gl_Position = p0 + rad * (cos(t) * v_vec + sin(t) * w_vec); 
}