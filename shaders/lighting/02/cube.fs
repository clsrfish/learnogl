#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_CubeColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main(){
    float ambientStrength=.1;
    vec3 ambient=ambientStrength*u_LightColor;
    
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(u_LightPos-FragPos);
    float diffStrength=max(dot(norm,lightDir),0.);
    vec3 diffuse=diffStrength*u_LightColor;
    
    float specularStrength=.5;
    vec3 viewDir=normalize(u_ViewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.),32);
    vec3 specular=spec*specularStrength*u_LightColor;
    
    vec3 result=(ambient+diffuse+specular)*u_CubeColor;
    FragColor=vec4(result,1.);
}