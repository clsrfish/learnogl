#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shineness;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main() {
    
    vec3 diffuseColor = vec3(texture(u_Material.diffuse, TexCoord));
    
    // ambient
    vec3 ambient = u_Light.ambient * diffuseColor;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_Light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * diffuseColor;
    
    // specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shineness);
    vec3 specular = u_Light.specular * spec * vec3(texture(u_Material.specular, TexCoord));
    
    // spotlight
    float theta = dot(lightDir, normalize(-u_Light.direction));
    float epsilon = u_Light.cutOff - u_Light.outerCutOff;
    float intensity = clamp((theta - u_Light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    
    float distance = length(u_Light.position - FragPos);
    float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance + u_Light.quadratic * (distance * distance));
    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}