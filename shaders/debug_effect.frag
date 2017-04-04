#version 330

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D colorMap;

uniform int type;

in vec2 uv;

layout (location = 0) out vec4 color;

void main()
{
    if(type == 0) // Position
    {
        color = vec4(texture(positionMap, uv).xyz, 1.);
    }
    else if(type == 1) // Normal
    {
        color = vec4(0.5 * normalize(texture(normalMap, uv).xyz) + 0.5, 1.);
    }
    else if(type == 2) // Color
    {
        color = vec4(texture(colorMap, uv).xyz, 1.);
    }
    else {
        color = vec4(0., 0., 0., 0.);
    }
}
