#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main(){
    vec4 texColor=texture(u_Texture,TexCoords);
    // discard is unnecessary, if blending enabled
    // if(texColor.a<.1){
        //     discard;
    // }
    FragColor=texColor;
}