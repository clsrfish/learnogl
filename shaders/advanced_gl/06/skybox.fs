#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube u_Texture;

void main() {
    FragColor = texture(u_Texture, TexCoords);
    // FragColor = vec4(1.0);
}