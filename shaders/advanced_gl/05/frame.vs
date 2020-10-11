#version 330 core

layout(location = 0)in vec2 a_Pos;
layout(location = 1)in vec2 a_TexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(a_Pos, 0.0, 1.0);
    TexCoords = a_TexCoords;
    
}