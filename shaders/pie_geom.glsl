#version 320 es

layout(points) in;
in VS_OUT {
    int index;
    int value;
} gs_in[];

uniform int _Size;

layout(triangle_strip, max_vertices = 3) out;
out GS_OUT {
    flat int index;
    flat vec3 color;
} fs_in;

const float PI = radians(180.0);

vec3 hsv(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    float angle1 = 2.0 * PI * float(gs_in[0].index) / float(_Size);
    float angle2 = 2.0 * PI * float(gs_in[0].index + 1) / float(_Size);

    vec3 color = hsv(vec3(float(gs_in[0].value) / float(_Size), 1, 1));

    gl_Position = vec4(0, 0, 0, 1);
    fs_in.index = gs_in[0].index;
    fs_in.color = color;
    EmitVertex();

    gl_Position = vec4(sin(angle1), cos(angle1), 0, 1);
    fs_in.index = gs_in[0].index;
    fs_in.color = color;
    EmitVertex();

    gl_Position = vec4(sin(angle2), cos(angle2), 0, 1);
    fs_in.index = gs_in[0].index;
    fs_in.color = color;
    EmitVertex();

    EndPrimitive();
}
