#version 320 es

layout(points) in;
in VS_OUT {
    int index;
} gs_in[];

uniform int _Size;

layout(triangle_strip, max_vertices = 4) out;
out GS_OUT {
    flat int index;
} fs_in;

void main() {
    vec4 pos = gl_in[0].gl_Position;
    float size = max(2.0 / float(_Size), 0.005);

    gl_Position = pos + vec4(1, 0, 0, 0) * size;
    fs_in.index = gs_in[0].index;
    EmitVertex();

    gl_Position = pos + vec4(0, 1, 0, 0) * size;
    fs_in.index = gs_in[0].index;
    EmitVertex();

    gl_Position = pos + vec4(0, -1, 0, 0) * size;
    fs_in.index = gs_in[0].index;
    EmitVertex();

    gl_Position = pos + vec4(-1, 0, 0, 0) * size;
    fs_in.index = gs_in[0].index;
    EmitVertex();

    EndPrimitive();
}
