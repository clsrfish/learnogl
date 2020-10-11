#version 330 core
layout(location = 0)in vec3 a_Pos;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 TexCoords;

void main() {
    vec4 pos = u_Projection * u_View * vec4(a_Pos, 1.0);
    gl_Position = pos.xyww;
    TexCoords = a_Pos;
}