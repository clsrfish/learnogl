#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexCoord;

out vec2 texCoord;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){
    gl_Position=uProjection*uView*uModel*vec4(aPos,1.);
    texCoord=vec2(aTexCoord.x,1.-aTexCoord.y);
}