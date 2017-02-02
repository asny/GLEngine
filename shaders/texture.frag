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
    vec4 c = texture(texture0, coords);
    if(c.w < 1.0)
        discard;
    position = vec4(pos, 1.0);
    color = c;
    normal = vec4(nor, 1.0);
}
