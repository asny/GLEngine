#version 330

uniform sampler2D colorMap;
uniform sampler2D depthMap;

in vec2 uv;

out vec4 fragColour;

void main()
{
   	fragColour = texture(colorMap, uv).rgba;
}
