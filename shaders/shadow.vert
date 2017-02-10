#version 330

uniform mat4 MVPMatrix;

in vec3 position;

void main()
{
    gl_Position = MVPMatrix * vec4(position, 1.0);
}
