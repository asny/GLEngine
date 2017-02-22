#version 330

uniform sampler2D colorMap;
uniform sampler2D depthMap;

in vec2 uv;

layout (location = 0) out vec4 color;

const vec3 fog_color = vec3(0.8, 0.8, 0.8);
const float fog_density = 0.2;

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
    
    float factor = 0.1 + 1.0 /exp( (dist * fog_density) * (dist * fog_density));
    factor = clamp( factor, 0.0, 1.0 );
    
    color = vec4(mix(fog_color, light_color, factor), 1.);
}
