#version 330

uniform sampler2D texture0;

in vec2 coords;
in vec3 nor;
in vec3 pos;

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 color;
layout (location = 2) out vec4 normal;

void main()
{
    position = vec4(pos, 1.0);
    color = texture(texture0, coords);
    normal = vec4(nor, 1.0);
}
