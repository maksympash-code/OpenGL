#version 330 core

in vec2 aPos;
in vec2 aUV;
out vec2 vTexCoords;

uniform mat4 uTransform;

void main() {
    vTexCoords = aUV;
    vTexCoords.y = 1.0 - vTexCoords.y;
    gl_Position = uTransform * vec4(aPos, 0.0, 1.0);
}