#version 330

uniform sampler2D positionMap;
uniform sampler2D noiseTexture;

uniform float time;
uniform float fogDensity;
uniform vec3 fogColor;
uniform float minVisibility;
uniform vec3 eyePosition;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
    vec4 pos = texture(positionMap, uv);
    float factor;
    if(pos.w < 1.)
    {
        factor = minVisibility;
    }
    else
    {
        float dist = distance(pos.xyz, eyePosition);
        float noise = texture(noiseTexture, uv).r;
        
        float x = dist * fogDensity + 0.15 * noise * cos(time + noise * 3.14f);
        factor = 1.0 / exp(x * x);
        factor = clamp( factor, minVisibility, 1.0 );
    }
    
    color = vec4(fogColor, 1. - factor);
}
