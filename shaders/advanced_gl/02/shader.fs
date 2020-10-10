#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main(){
    FragColor=vec4(texture(u_Texture,TexCoords).rgb,1.);
}