#version 330 core
layout(location=0)in vec3 a_Pos;
layout(location=1)in vec3 a_Normal;
layout(location=2)in vec2 a_TexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main(){
    gl_Position=u_Projection*u_View*u_Model*vec4(a_Pos,1.);
    FragPos=(u_Model*vec4(a_Pos,1.)).xyz;
    Normal=Normal=mat3(transpose(inverse(u_Model)))*a_Normal;;
    TexCoords=a_TexCoords;
}