#version 330

uniform sampler2D colorMap;
uniform sampler2D depthMap;

in vec2 uv;

out vec4 fragColour;

void main()
{
    float depth = texture(depthMap, uv).r;
    if(depth == 1.)
        discard;
    
   	fragColour = texture(colorMap, uv).rgba;
    gl_FragDepth = depth;
}
