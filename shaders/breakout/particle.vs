#version 330 core

layout(location = 0)in vec4 a_Vertex;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;

void main() {
    float scale = 10.0f;
    TexCoords = a_Vertex.zw;
    ParticleColor = color;
    gl_Position = projection * vec4((a_Vertex.xy * scale + offset), 0.0, 1.0f);
}