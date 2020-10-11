#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

const float offset=1./300.;

void main()
{
    // FragColor=texture(u_Texture,TexCoords);
    
    // inversion
    // FragColor=vec4((1.-texture(u_Texture,TexCoords)).rgb,1.);
    
    // grayscale
    // FragColor=texture(u_Texture,TexCoords);
    // float average=(FragColor.r+FragColor.g+FragColor.b)/3.;
    // float average=.2126*FragColor.r+.7152*FragColor.g+.0722*FragColor.b;
    // FragColor=vec4(average,average,average,1.);
    
    // kernel
    vec2 offsets[9]=vec2[](
        vec2(-offset,offset),
        vec2(0.,offset),
        vec2(offset,offset),
        vec2(-offset,0.),
        vec2(0.,0.),
        vec2(offset,0.),
        vec2(-offset,-offset),
        vec2(0.,-offset),
        vec2(offset,-offset)
    );
    // sharpen kernel
    // float kernel[9]=float[](
        //     -1,-1,-1,
        //     -1,9,-1,
        //     -1,-1,-1
    // );
    // blur kernel
    // float kernel[9]=float[](
        //     1./16,2./16,1./16,
        //     2./16,4./16,2./16,
        //     1./16,2./16,1./16
    // );
    // edge detection
    float kernel[9]=float[](
        1,1,1,
        1,-8,1,
        1,1,1
    );
    vec3 sampleTex[9];
    for(int i=0;i<9;i++){
        sampleTex[i]=vec3(texture(u_Texture,TexCoords.st+offsets[i]));
    }
    vec3 color=vec3(0.);
    for(int i=0;i<9;i++){
        color+=sampleTex[i]*kernel[i];
    }
    
    FragColor=vec4(color,1.);
}