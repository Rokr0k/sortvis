
#version 320 es

#ifdef GL_ES
precision highp float;
#endif

in GS_OUT {
    flat int index;
} fs_in;

uniform int _Behavior;
uniform int _Arg0;
uniform int _Arg1;

out vec4 outColor;

void main() {
    bool b1 = _Behavior == 1 && (fs_in.index == _Arg0 || fs_in.index == _Arg1);
    bool b2 = _Behavior == 2 && (fs_in.index == _Arg0 || fs_in.index == _Arg1);
    bool b3 = _Behavior == 3 && fs_in.index == _Arg0;

    outColor = vec4(1);

    outColor += (vec4(0, 0, 1, 1) - outColor) * float(b1);
    outColor += (vec4(1, 0, 0, 1) - outColor) * float(b2);
    outColor += (vec4(0, 1, 0, 1) - outColor) * float(b3);
}
