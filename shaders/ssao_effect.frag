#version 330

uniform sampler2D positionMap;
uniform sampler2D normalMap;

uniform sampler2D sampleTexture;
uniform sampler2D noiseTexture;
uniform mat4 VPBMatrix;
uniform vec3 eyePosition;
uniform vec2 noiseUvScale;
uniform int sampleSize;
uniform float radius;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
    vec3 pos = texture(positionMap, uv).xyz;
    float depth = distance(pos, eyePosition);
    
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    vec3 random_dir = texture(noiseTexture, uv * noiseUvScale).xyz;
    vec3 tangent = normalize(random_dir - normal * dot(random_dir, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);
    
    float occlusion = 0.0;
    for (int i = 0; i < sampleSize; i++)
    {
        // Sample position and depth
        vec3 sample_pos = pos + radius * tbn * texture(sampleTexture, vec2((i + 0.5)/sampleSize, 0.5)).xyz;
        vec4 screen_sample_pos = VPBMatrix * vec4(sample_pos, 1.);
        float sample_depth = distance(texture(positionMap, screen_sample_pos.xy / screen_sample_pos.w).xyz, eyePosition);
        
        // Range check and accumulate
        if(abs(depth - sample_depth) < radius && sample_depth + 0.025 <= distance(sample_pos, eyePosition))
        {
            occlusion += 1.0;
        }
    }
    occlusion = occlusion / sampleSize;
    
    color = vec4(0., 0., 0., occlusion);
}
