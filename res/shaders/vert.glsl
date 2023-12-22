#version 330 core

layout (location = 0) in vec4 a_position;
out vec4 coord;
// layout (location = 1) in vec4 a_color;

void main() {
    gl_Position = a_position;
    vec4 z = (a_position + 1.0) / 2.0; 
    coord = z;
}

