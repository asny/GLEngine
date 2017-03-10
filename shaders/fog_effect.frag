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

// factor: 1 == full fog, 0 == no fog
void main()
{
    vec4 pos = texture(positionMap, uv);
    
    // Distance
    float dist = min(distance(pos.xyz, eyePosition), 100.);
    float a = eyePosition.y;
    float b = pos.y;
    float t1 = clamp(-b/(a-b), 0., 1.);
    float t2 = clamp((noFogHeight-b)/(a-b), 0., 1.);
    dist *= abs(t1 - t2);
    
    float x = dist * fogDensity;
    float factor = 1. - 1. / exp(x * x);
    
    // Noise
//    float noise = texture(noiseTexture, 0.1 * pos.xz).r;
//    factor += 0.1 * cos(time + 0.4 * 3.14 * noise);
//    factor = clamp(factor, 0., 1.);
    
    // Output
    color = vec4(fogColor, factor);
}
