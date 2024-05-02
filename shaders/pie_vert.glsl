#version 320 es

layout(location = 0) in int aValue;

uniform int _Size;

out VS_OUT {
    int index;
    int value;
} gs_in;

void main() {
    gl_Position = vec4(0, 0, 0, 1);
    gs_in.index = gl_VertexID;
    gs_in.value = aValue;
}
