#version 330

uniform sampler2D positionMap;
uniform sampler2D noiseTexture;

uniform float time;
uniform float fogDensity;
uniform vec3 fogColor;
uniform float noFogHeight;
uniform vec3 eyePosition;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
    vec4 pos = texture(positionMap, uv);
    float factor = 0.;
    if(pos.w == 1.)
    {
        float dist = distance(pos.xyz, eyePosition);
        float noise = texture(noiseTexture, 0.1 * pos.xz).r;
        
        float t = time + 0.4 * 3.14 * noise;
        float x = (1. + 0.1 * cos(t)) * dist * fogDensity;
        factor = 1.0 / exp(x * x);
        factor = clamp( factor, 0., 1. );
    }
    factor = 1. - factor; // 1: full fog, 0: no fog
    float height = mix(eyePosition.y, pos.y, 0.5);
    if(height > 0.)
    {
        factor *= 1. - clamp( height * height / (noFogHeight * noFogHeight) , 0., 1.);
    }
    
    color = vec4(fogColor, factor);
}
