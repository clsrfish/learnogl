#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shineness;
};

struct Light{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main(){
    
    vec3 diffuseColor=vec3(texture(u_Material.diffuse,TexCoord));
    
    vec3 ambient=u_Light.ambient*diffuseColor;
    
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(-u_Light.direction);;
    float diff=max(dot(norm,lightDir),0.);
    vec3 diffuse=u_Light.diffuse*diff*diffuseColor;
    
    vec3 viewDir=normalize(u_ViewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.),u_Material.shineness);
    vec3 specular=u_Light.specular*spec*vec3(texture(u_Material.specular,TexCoord));
    
    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.);
}