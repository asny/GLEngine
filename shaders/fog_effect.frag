#version 330

uniform sampler2D colorMap;
uniform sampler2D depthMap;
uniform sampler2D noiseTexture;
uniform float time;

in vec2 uv;

layout (location = 0) out vec4 color;

const vec3 fog_color = vec3(0.8, 0.8, 0.8);
const float fog_density = 0.2;
const float min_visibility = 0.05;

float linear_depth(float depth)
{
    float zNear = 0.1f;
    float zFar = 100.f;
    depth = 2.0 * depth - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
   	vec3 light_color = texture(colorMap, uv).rgb;
    float depth = texture(depthMap, uv).r;
    float dist = linear_depth(depth);
    float noise = texture(noiseTexture, uv).r;
    
    float x = dist * fog_density + 0.15 * noise * cos(time + noise * 3.14f);
    float factor = 1.0 / exp(x * x);
    factor = clamp( factor, min_visibility, 1.0 );
    
    color = vec4(mix(fog_color, light_color, factor), 1.);
}
