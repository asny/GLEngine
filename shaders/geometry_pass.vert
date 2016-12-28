#version 330

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat4 NMatrix;

in vec3 position;
in vec3 normal;

out vec3 pos;
out vec3 nor;

void main()
{
    pos = (MVMatrix * vec4(position, 1.)).xyz;
    nor = mat3(NMatrix) * normal;
    gl_Position = MVPMatrix * vec4(position, 1.0);
}
