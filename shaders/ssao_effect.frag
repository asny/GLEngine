#version 330

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform sampler2D sampleTexture;
uniform sampler2D noiseTexture;
uniform mat4 VMatrix;
uniform mat4 PMatrix;

in vec2 uv;

layout (location = 0) out vec4 color;

const int sample_size = 4;
const float radius = 1.f;
const int WIN_SIZE_X = 2400;
const int WIN_SIZE_Y = 1400;

void main()
{
    vec3 pos = texture(positionMap, uv).xyz;
    float depth = (VMatrix * vec4(pos, 1.f)).z;
    
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    vec3 random_dir = texture(noiseTexture, vec2(uv.x * WIN_SIZE_X / sample_size, uv.y * WIN_SIZE_Y / sample_size)).xyz;
    vec3 tangent = normalize(random_dir - normal * dot(random_dir, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);
    
    float occlusion = 0.0;
    for (int i = 0; i < sample_size*sample_size; i++)
    {
        // get sample position:
        float x = mod(i, sample_size) / sample_size;
        float y = (i - mod(i, sample_size)) / (sample_size * sample_size);
        vec3 sample_pos = pos + radius * tbn * texture(sampleTexture, vec2(x,y)).xyz;
        sample_pos = (VMatrix * vec4(sample_pos, 1.)).xyz;
        
        // project sample position:
        vec4 offset = PMatrix * vec4(sample_pos, 1.0);
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;
        
        // get sample depth:
        float sample_depth = texture(depthMap, offset.xy).r;
        
        // range check & accumulate:
        if(abs(depth - sample_depth) < radius && sample_depth <= sample_pos.z)
        {
            occlusion += 1.0;
        }
        occlusion += sample_depth;
    }
    occlusion = occlusion / (sample_size * sample_size);
    
    color = vec4(0., 0., 0., 0.);
}
