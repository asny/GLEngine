#version 330

uniform mat4 MVPMatrix;

in vec3 position;
in vec2 uv_coordinates;

out vec2 coords;

void main()
{
    // Pass the tex coord straight through to the fragment shader
    coords = uv_coordinates;
    
    // Calculate position
    gl_Position = MVPMatrix * vec4(position, 1.);
}
