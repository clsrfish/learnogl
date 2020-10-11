#version 330 core
layout(location = 0)in vec2 a_Pos;
layout(location = 1)in vec3 a_Color;

out vec3 Color;

uniform vec2 offsets[100];

void main() {
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(a_Pos + offset, 0.0, 1.0);
    Color = a_Color;
}