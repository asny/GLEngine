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
const float radius = 0.1f;
const int WIN_SIZE_X = 2400;
const int WIN_SIZE_Y = 1400;

void main()
{
    vec3 pos = texture(positionMap, uv).xyz;
    vec4 screen_pos = PMatrix * VMatrix * vec4(pos, 1.f);
    float depth = screen_pos.z / screen_pos.w;
    
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
        vec4 screen_sample_pos = PMatrix * VMatrix * vec4(sample_pos, 1.);
        float sample_pos_depth = screen_sample_pos.z / screen_sample_pos.w;
        
        // project sample position:
        vec2 offset = 0.5 * screen_sample_pos.xy/screen_sample_pos.w + 0.5;
        
        // get sample depth:
        float sample_depth = texture(depthMap, offset.xy).r;
        
        // range check & accumulate:
        if(abs(depth - sample_depth) < radius && sample_depth <= sample_pos_depth)
        {
            occlusion += 1.0;
        }
    }
    occlusion = occlusion / (sample_size * sample_size);
    
    color = vec4(0., 0., 0., occlusion);
}
