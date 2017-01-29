#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec2 screenSize;
uniform vec3 eyePosition;

out vec4 fragColour;

struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation attenuation;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float cutoff;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform int lightType;

const float specularIntensity = 0.f;
const float specularPower = 5.f;

vec4 calculate_light(BaseLight light,
                       vec3 lightDirection,
                       vec3 position,
                       vec3 normal)
{
    vec4 AmbientColor = vec4(light.color * light.ambientIntensity, 1.0);
    float DiffuseFactor = dot(normal, -lightDirection);
    
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
    
    if (DiffuseFactor > 0.0)
    {
        DiffuseColor = vec4(light.color * light.diffuseIntensity * DiffuseFactor, 1.0);
        
        vec3 VertexToEye = normalize(eyePosition - position);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float SpecularFactor = dot(VertexToEye, lightReflect);
        if (SpecularFactor > 0.0)
        {
            SpecularFactor = pow(SpecularFactor, specularPower);
            SpecularColor = vec4(light.color * specularIntensity * SpecularFactor, 1.0);
        }
    }
    
    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 calculate_directional_light(vec3 position, vec3 normal)
{
    return calculate_light(directionalLight.base,
                             directionalLight.direction,
                             position,
                             normal);
}

vec4 calculate_point_light(vec3 position, vec3 normal)
{
    vec3 lightDirection = position - pointLight.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);
    
    vec4 color = calculate_light(pointLight.base, lightDirection, position, normal);
    
    float attenuation =  pointLight.attenuation.constant +
        pointLight.attenuation.linear * distance +
        pointLight.attenuation.exp * distance * distance;
    
    attenuation = max(1.0, attenuation);
    
    return color / attenuation;
}

void main()
{
   	vec2 uv = gl_FragCoord.xy / screenSize;
    float depth = texture(depthMap, uv).r;
    if(depth == 1.)
        discard;
    
   	vec3 pos = texture(positionMap, uv).xyz;
   	vec3 color = texture(colorMap, uv).xyz;
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    
    vec4 light;
    if(lightType == 1)
    {
        light = calculate_directional_light(pos, normal);
    }
    else if(lightType == 2)
    {
        light = calculate_point_light(pos, normal);
    }
    
    fragColour = vec4(color, 1.0) * light;
    gl_FragDepth = depth;
}
