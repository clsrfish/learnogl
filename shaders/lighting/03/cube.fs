#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shineness;
};

struct Light {
    vec3 pos;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main() {
    
    vec3 ambient = u_Light.ambient * u_Material.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_Light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * u_Material.diffuse;
    
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shineness);
    vec3 specular = u_Light.specular * spec * u_Material.specular;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}