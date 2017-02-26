#version 330

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
   	vec3 light_color = texture(colorMap, uv).rgb;
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    float depth = texture(depthMap, uv).r;
    
    color = vec4(light_color, 1.);
}
