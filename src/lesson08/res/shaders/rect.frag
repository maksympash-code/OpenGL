#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

uniform float uT;

out vec4 FragColor;

void main() {
    vec4 color0 = texture(uTexture0, vTexCoords);
    vec4 color1 = texture(uTexture1, vTexCoords);
    vec4 color2 = texture(uTexture2, vTexCoords);
    FragColor = mix(color0, color1, uT);  // колір отриманий з текстури
}