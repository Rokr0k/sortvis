#version 330 core

layout(points) in;
in VS_OUT {
    int index;
    vec4 color;
} gs_in[];

uniform int _Size;

layout(triangle_strip, max_vertices = 3) out;
out GS_OUT {
    vec4 color;
} fs_in;

const float PI = radians(180.0);

void main() {
    float angle1 = 2.0 * PI * float(gs_in[0].index) / float(_Size);
    float angle2 = 2.0 * PI * float(gs_in[0].index + 1) / float(_Size);

    fs_in.color = gs_in[0].color;

    gl_Position = vec4(0, 0, 0, 1);
    EmitVertex();

    gl_Position = vec4(sin(angle1), cos(angle1), 0, 1);
    EmitVertex();

    gl_Position = vec4(sin(angle2), cos(angle2), 0, 1);
    EmitVertex();

    EndPrimitive();
}
