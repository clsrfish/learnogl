#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexCoord;

out vec2 texCoord;
uniform mat4 uTransform;

void main(){
    gl_Position=uTransform*vec4(aPos,1.);
    texCoord=vec2(aTexCoord.x,1.-aTexCoord.y);
}
