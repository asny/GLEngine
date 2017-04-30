#version 330

uniform sampler2D positionMap;
uniform sampler2D normalMap;

uniform vec3 samples[4];
uniform int sampleSize;
uniform sampler2D noiseTexture;
uniform sampler2D blueNoiseTexture0;
uniform sampler2D blueNoiseTexture1;
uniform sampler2D blueNoiseTexture2;
uniform sampler2D blueNoiseTexture3;
uniform int noiseSize;

uniform mat4 VPBMatrix;
uniform vec3 eyePosition;
uniform float radius;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
    vec3 pos = texture(positionMap, uv).xyz;
    float depth = distance(pos, eyePosition);
    
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    vec2 noise_uv = mod(gl_FragCoord.xy, noiseSize) / noiseSize;
    vec3 random_dir = texture(noiseTexture, noise_uv).xyz;
    vec3 tangent = normalize(random_dir - normal * dot(random_dir, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);
    
    float occlusion = 0.0;
    for (int i = 0; i < sampleSize; i++)
    {
        // Sample position and depth
        float random_radius;
        if(mod(i, 4) == 0)
            random_radius = texture(blueNoiseTexture0, gl_FragCoord.xy/128.).x;
        if(mod(i, 4) == 1)
            random_radius = texture(blueNoiseTexture1, gl_FragCoord.xy/128.).x;
        if(mod(i, 4) == 2)
            random_radius = texture(blueNoiseTexture2, gl_FragCoord.xy/128.).x;
        if(mod(i, 4) == 3)
            random_radius = texture(blueNoiseTexture3, gl_FragCoord.xy/128.).x;
        vec3 sample_pos = pos + radius * tbn * samples[i] * random_radius * random_radius;
        
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
