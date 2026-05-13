#version 330 core

//in vec3 color;
uniform vec4 uColor;
out vec4 FragColor; // колір фрагменту

void main() {
//     FragColor = vec4(0.0, 1.0, 0.0, 1.0); // зелений
    FragColor = uColor; // колір отриманий з юніформи
}