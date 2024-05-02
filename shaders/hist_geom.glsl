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
    vec2 size = vec2(2.0 / float(_Size), -1.0 - pos.y);

    gl_Position = pos;
    fs_in.index = gs_in[0].index;
    EmitVertex();

    gl_Position = pos + vec4(size * vec2(1, 0), 0, 0);
    fs_in.index = gs_in[0].index;
    EmitVertex();

    gl_Position = pos + vec4(size * vec2(0, 1), 0, 0);
    fs_in.index = gs_in[0].index;
    EmitVertex();

    gl_Position = pos + vec4(size, 0, 0);
    fs_in.index = gs_in[0].index;
    EmitVertex();

    EndPrimitive();
}
