#version 330

uniform mat4 MVMatrix;

in vec3 position;

void main()
{
    // Calculate position
    gl_Position = MVMatrix * vec4(position, 1.);
}
