#version 330

uniform sampler2D noiseTexture;

in vec2 dens;
in vec3 nor;
in vec3 pos;

layout (location = 0) out vec4 color;

void main()
{
    float noise = texture(noiseTexture, pos.xz).x;
    float sand = dens.x;
    if(sand >= 0. && sand <= 1.)
    {
        color = vec4(vec3(0.5, 0.5, 0.4) + 0.5 * vec3(noise, noise, noise), sand);
    }
    else {
        discard;
    }
}
