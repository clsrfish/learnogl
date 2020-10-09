#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shineness;
};

struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
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

uniform DirLight u_DirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

vec3 calcDirLight(DirLight light,vec3 normal,vec3 viewDir);

vec3 calcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

vec3 calcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

void main(){
    
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(u_ViewPos-FragPos);
    
    vec3 result=calcDirLight(u_DirLight,norm,viewDir);
    // vec3 result=vec3(0.);
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        result+=calcPointLight(u_PointLights[i],norm,FragPos,viewDir);
    }
    result+=calcSpotLight(u_SpotLight,norm,FragPos,viewDir);
    
    FragColor=vec4(result,1.);
}

vec3 calcDirLight(DirLight light,vec3 normal,vec3 viewDir){
    vec3 lightDir=normalize(-light.direction);
    // diffuse
    float diff=max(dot(normal,lightDir),0.);
    
    // specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(reflectDir,viewDir),0.),u_Material.shineness);
    
    // combine
    vec3 ambient=light.ambient*vec3(texture(u_Material.diffuse,TexCoord));
    vec3 diffuse=light.diffuse*diff*vec3(texture(u_Material.diffuse,TexCoord));
    vec3 specular=light.specular*spec*vec3(texture(u_Material.specular,TexCoord));
    
    return(ambient+diffuse+specular);
    
}

vec3 calcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir=normalize(light.position-fragPos);
    // diffuse
    float diff=max(dot(normal,lightDir),0.);
    
    // specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(reflectDir,viewDir),0.),u_Material.shineness);
    
    // combine
    vec3 ambient=light.ambient*vec3(texture(u_Material.diffuse,TexCoord));
    vec3 diffuse=light.diffuse*diff*vec3(texture(u_Material.diffuse,TexCoord));
    vec3 specular=light.specular*spec*vec3(texture(u_Material.specular,TexCoord));
    
    // attenuation
    float distance=length(light.position-fragPos);
    float attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    
    return(ambient+diffuse+specular);
}

vec3 calcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir=normalize(light.position-fragPos);
    // diffuse
    float diff=max(dot(normal,lightDir),0.);
    
    // specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(reflectDir,viewDir),0.),u_Material.shineness);
    
    // combine
    vec3 ambient=light.ambient*vec3(texture(u_Material.diffuse,TexCoord));
    vec3 diffuse=light.diffuse*diff*vec3(texture(u_Material.diffuse,TexCoord));
    vec3 specular=light.specular*spec*vec3(texture(u_Material.specular,TexCoord));
    
    // spotlight
    float theta=dot(lightDir,normalize(-light.direction));
    float epsilon=light.cutOff-light.outerCutOff;
    float intensity=clamp((theta-light.outerCutOff)/epsilon,0.,1.);
    diffuse*=intensity;
    specular*=intensity;
    
    // attenuation
    float distance=length(light.position-fragPos);
    float attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    
    // return(ambient+diffuse+specular)*attenuation;
    return(ambient+diffuse+specular);
}