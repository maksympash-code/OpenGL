#version 330 core

in vec4 aPos;
uniform vec4 uShift;

void main() {
    gl_Position = aPos + uShift;
}