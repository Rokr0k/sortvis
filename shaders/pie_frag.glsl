#version 330 core

in GS_OUT {
    vec4 color;
} fs_in;

out vec4 outColor;

void main() {
    outColor = fs_in.color;
}
