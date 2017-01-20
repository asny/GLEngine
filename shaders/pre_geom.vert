#version 330

uniform mat4 MMatrix;

in vec3 position;

void main()
{
    // Calculate position
    gl_Position = MMatrix * vec4(position, 1.);
}
