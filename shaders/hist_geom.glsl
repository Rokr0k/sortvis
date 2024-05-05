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
    vec2 size = vec2(2.0 / float(_Size), -1.0 - pos.y);

    fs_in.color = gs_in[0].color;

    gl_Position = pos;
    EmitVertex();

    gl_Position = pos + vec4(size * vec2(1, 0), 0, 0);
    EmitVertex();

    gl_Position = pos + vec4(size * vec2(0, 1), 0, 0);
    EmitVertex();

    gl_Position = pos + vec4(size, 0, 0);
    EmitVertex();

    EndPrimitive();
}
