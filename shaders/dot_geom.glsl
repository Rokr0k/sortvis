#version 330 core

layout(points) in;
in VS_OUT {
    vec4 color;
} gs_in[];

uniform int _Size;

layout(triangle_strip, max_vertices = 4) out;
out GS_OUT {
    vec4 color;
} fs_in;

void main() {
    vec4 pos = gl_in[0].gl_Position;
    float size = max(2.0 / float(_Size), 0.005);

    fs_in.color = gs_in[0].color;

    gl_Position = pos + vec4(1, 0, 0, 0) * size;
    EmitVertex();

    gl_Position = pos + vec4(0, 1, 0, 0) * size;
    EmitVertex();

    gl_Position = pos + vec4(0, -1, 0, 0) * size;
    EmitVertex();

    gl_Position = pos + vec4(-1, 0, 0, 0) * size;
    EmitVertex();

    EndPrimitive();
}
