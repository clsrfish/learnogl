#version 330 core

layout(location = 0)in vec4 a_Vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main() {
    TexCoords = a_Vertex.zw;
    gl_Position = projection * model * vec4(a_Vertex.xy, 0.0f, 1.0f);
}