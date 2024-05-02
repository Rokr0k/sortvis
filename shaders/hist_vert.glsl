#version 320 es

layout(location = 0) in int aValue;

uniform int _Size;

out VS_OUT {
    int index;
} gs_in;

void main() {
    float x = float(gl_VertexID) / float(_Size) * 2.0 - 1.0;
    float y = float(aValue + 1) / float(_Size) * 2.0 - 1.0;
    gl_Position = vec4(x, y, 0, 1);
    gs_in.index = gl_VertexID;
}
