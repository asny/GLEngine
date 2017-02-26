#version 330

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform sampler2D sampleTexture;
uniform sampler2D noiseTexture;
uniform vec2 noiseScale;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
   	vec3 light_color = texture(colorMap, uv).rgb;
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    float depth = texture(depthMap, uv).r;
    
    vec3 rvec = texture(noiseTexture, uv).xyz;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);
    
    
    vec3 kernel_sample = texture(sampleTexture, uv).xyz;
    color = vec4(normal, 1.);
}
