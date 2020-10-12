#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 u_ViewPos;
uniform sampler2D u_TextureDiffuse0;

void main()
{
    FragColor = (texture(u_TextureDiffuse0, TexCoords));
}