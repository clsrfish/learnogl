#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_Texture;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z*(far - near));
}

void main() {
    // FragColor = vec4(texture(u_Texture, TexCoords).rgb, 1.0);
    
    // non linear
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    
    // linear
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0);
}