#version 330
                                                                                    
in vec3 position;
in vec2 uv_coordinates;

out vec2 uv;

void main()
{
    uv = uv_coordinates;
    gl_Position = vec4(position, 1.0);
}
