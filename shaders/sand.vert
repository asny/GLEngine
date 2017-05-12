#version 330

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 NMatrix;

in vec3 position;
in vec2 density;
in vec3 normal;

out vec2 dens;
out vec3 pos;
out vec3 nor;

void main()
{
    dens = density;
    pos = (MMatrix * vec4(position, 1.)).xyz;
    nor = mat3(NMatrix) * normal;
    
    // Calculate position
    gl_Position = MVPMatrix * vec4(position, 1.);
}
