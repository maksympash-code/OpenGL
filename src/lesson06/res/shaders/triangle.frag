#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTexture;

out vec4 FragColor;

void main() {
    vec4 color = texture(uTexture, vTexCoords);
    FragColor = color; // колір отриманий з текстури
}