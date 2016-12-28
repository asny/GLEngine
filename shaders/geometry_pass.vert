#version 330
                                                                                    
in vec3 position;                                            
in vec3 normal;

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;

out vec3 nor;
out vec3 pos;


void main()
{
    nor        = (MMatrix * vec4(normal, 0.0)).xyz;
    pos      = (MMatrix * vec4(position, 1.0)).xyz;
    gl_Position    = MVPMatrix * vec4(position, 1.0);
}
