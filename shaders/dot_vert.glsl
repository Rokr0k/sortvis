#version 330 core

layout(location = 0) in int aValue;

uniform int _Size;
uniform int _Behavior;
uniform int _Arg0;
uniform int _Arg1;

out VS_OUT {
    vec4 color;
} gs_in;

void main() {
    float x = float(gl_VertexID) / float(_Size - 1) * 2.0 - 1.0;
    float y = float(aValue) / float(_Size - 1) * 2.0 - 1.0;

    bool b1 = _Behavior == 1 && (gl_VertexID == _Arg0 || gl_VertexID == _Arg1);
    bool b2 = _Behavior == 2 && (gl_VertexID == _Arg0 || gl_VertexID == _Arg1);
    bool b3 = _Behavior == 3 && gl_VertexID == _Arg0;

    vec4 color = vec4(1);

    color += (vec4(0, 0, 1, 1) - color) * float(b1);
    color += (vec4(1, 0, 0, 1) - color) * float(b2);
    color += (vec4(0, 1, 0, 1) - color) * float(b3);

    gl_Position = vec4(x, y, 0, 1);

    gs_in.color = color;
}
