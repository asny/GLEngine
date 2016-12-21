#version 150

uniform mat4 MVMatrix;
uniform mat4 PMatrix;

in vec3 position;

out vec3 coords;

void main()
{
    coords = position;
    
    // Calculate position
    gl_Position = (PMatrix * mat4(mat3(MVMatrix)) * vec4(position, 1.)).xyww;
}