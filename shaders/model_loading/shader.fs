#version 330 core

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 u_ViewPos;
uniform sampler2D u_TextureDiffuse0;
uniform sampler2D u_TextureSpecular0;
#define NR_POINT_LIGHTS 2
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 result = vec3(0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i ++ ) {
        result += calcPointLight(u_PointLights[i], normal, FragPos, viewDir);
    }
    FragColor = vec4(result, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32.0);
    
    // combine
    vec3 ambient = light.ambient * vec3(texture(u_TextureDiffuse0, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_TextureDiffuse0, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_TextureSpecular0, TexCoords));
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}