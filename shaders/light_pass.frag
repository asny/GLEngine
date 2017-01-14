#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec2 screenSize;
uniform vec3 lightDirection;

out vec4 fragColour;

void main()
{
   	vec2 uv = gl_FragCoord.xy / screenSize;
    float depth = texture(depthMap, uv).r;
    if(depth == 1.)
        discard;
    
   	vec3 pos = texture(positionMap, uv).xyz;
   	vec3 color = texture(colorMap, uv).xyz;
   	vec3 Normal = texture(normalMap, uv).xyz;
    
    float specPow = 5.;
    
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDirection);
    vec3 E = normalize(-pos);
    vec3 R = normalize(reflect(-L,N));
    
    // Calculate colour
    float ambient = 0.2;
    float diffuse = clamp( max(dot(N,L), 0.0) , 0.0, 1.0 );
    float spec = clamp ( pow(max(dot(R,E), 0.0), specPow) , 0.0, 1.0 );
    fragColour = vec4(color * (ambient + diffuse + spec), 1.);
    gl_FragDepth = depth;
}
