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
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform int lightType;

const float specularIntensity = 0.f;
const float specularPower = 5.f;

vec4 CalcLightInternal(BaseLight Light,
                       vec3 LightDirection,
                       vec3 WorldPos,
                       vec3 Normal)
{
    vec4 AmbientColor = vec4(Light.color * Light.ambientIntensity, 1.0);
    float DiffuseFactor = dot(Normal, -LightDirection);
    
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
    
    if (DiffuseFactor > 0.0)
    {
        DiffuseColor = vec4(Light.color * Light.diffuseIntensity * DiffuseFactor, 1.0);
        
        vec3 VertexToEye = normalize(eyePosition - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        if (SpecularFactor > 0.0)
        {
            SpecularFactor = pow(SpecularFactor, specularPower);
            SpecularColor = vec4(Light.color * specularIntensity * SpecularFactor, 1.0);
        }
    }
    
    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(directionalLight.base,
                             directionalLight.direction,
                             WorldPos,
                             Normal);
}

/*vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)
{
    vec3 LightDirection = WorldPos - gPointLight.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);
    
    vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, WorldPos, Normal);
    
    float Attenuation =  pointLight.Atten.Constant +
    gPointLight.Atten.Linear * Distance +
    gPointLight.Atten.Exp * Distance * Distance;
    
    Attenuation = max(1.0, Attenuation);
    
    return Color / Attenuation;
}*/

void main()
{
   	vec2 uv = gl_FragCoord.xy / screenSize;
    float depth = texture(depthMap, uv).r;
    if(depth == 1.)
        discard;
    
   	vec3 pos = texture(positionMap, uv).xyz;
   	vec3 color = texture(colorMap, uv).xyz;
   	vec3 normal = normalize(texture(normalMap, uv).xyz);
    
    fragColour = vec4(color, 1.0) * CalcDirectionalLight(pos, normal);
    gl_FragDepth = depth;
}
