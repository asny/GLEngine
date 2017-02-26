#version 330

uniform sampler2D depthMap;
uniform sampler2D noiseTexture;

uniform float time;
uniform float fogDensity;
uniform vec3 fogColor;
uniform float minVisibility;
uniform float zNear;
uniform float zFar;

in vec2 uv;

layout (location = 0) out vec4 color;

float linear_depth(float depth)
{
    depth = 2.0 * depth - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
    float depth = texture(depthMap, uv).r;
    float dist = linear_depth(depth);
    float noise = texture(noiseTexture, uv).r;
    
    float x = dist * fogDensity + 0.15 * noise * cos(time + noise * 3.14f);
    float factor = 1.0 / exp(x * x);
    factor = clamp( factor, minVisibility, 1.0 );
    
    color = vec4(fogColor, factor);
}
