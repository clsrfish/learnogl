#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

const float offset = 1.0 / 300.0;

void main()
{
    // FragColor = texture(u_Texture, TexCoords);
    
    // inversion
    // FragColor = vec4((1.0 - texture(u_Texture, TexCoords)).rgb, 1.0);
    
    // grayscale
    // FragColor = texture(u_Texture, TexCoords);
    // float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(average, average, average, 1.0);
    
    // kernel
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0),
        vec2(0.0, 0.0),
        vec2(offset, 0.0),
        vec2(-offset, - offset),
        vec2(0.0, - offset),
        vec2(offset, - offset)
    );
    // sharpen kernel
    // float kernel[9] = float[](
        //     - 1, - 1, - 1,
        //     - 1, 9, - 1,
        //     - 1, - 1, - 1
    // );
    // blur kernel
    // float kernel[9] = float[](
        //     1.0 / 16, 2.0 / 16, 1.0 / 16,
        //     2.0 / 16, 4.0 / 16, 2.0 / 16,
        //     1.0 / 16, 2.0 / 16, 1.0 / 16
    // );
    // edge detection
    float kernel[9] = float[](
        1, 1, 1,
        1, - 8, 1,
        1, 1, 1
    );
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i ++ ) {
        sampleTex[i] = vec3(texture(u_Texture, TexCoords.st + offsets[i]));
    }
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i ++ ) {
        color += sampleTex[i] * kernel[i];
    }
    
    FragColor = vec4(color, 1.0);
}