#version 330

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 NMatrix;

in vec3 position;
in vec2 uv_coordinates;
in vec3 normal;

out vec2 coords;
out vec3 pos;
out vec3 nor;

void main()
{
    // Pass the tex coord straight through to the fragment shader
    coords = uv_coordinates;
    pos = (MMatrix * vec4(position, 1.)).xyz;
    nor = mat3(NMatrix) * normal;
    
    // Calculate position
    gl_Position = MVPMatrix * vec4(position, 1.);
}
