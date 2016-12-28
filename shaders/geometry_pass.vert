#version 330
                                                                                    
layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 TexCoord;                                             
//layout (location = 2) in vec3 Normal;                                               

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
                                        
//out vec2 TexCoord0;                                                                 
//out vec3 Normal0;                                                                   
out vec3 pos;


void main()
{
//    TexCoord0      = TexCoord;                  
//    Normal0        = (gWorld * vec4(Normal, 0.0)).xyz;   
    pos      = (MMatrix * vec4(position, 1.0)).xyz;
    gl_Position    = MVPMatrix * vec4(position, 1.0);
}
