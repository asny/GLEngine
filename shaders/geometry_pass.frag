#version 330
                                                                        
//in vec2 TexCoord0;                                                                  
//in vec3 Normal0;                                                                    
in vec3 pos;

layout (location = 0) out vec4 position;
//layout (location = 1) out vec3 DiffuseOut;     
//layout (location = 2) out vec3 NormalOut;     
//layout (location = 3) out vec3 TexCoordOut;    

//uniform sampler2D gColorMap;                

void main()									
{											
	position     = vec4(pos, 1.0);
//	DiffuseOut      = texture(gColorMap, TexCoord0).xyz;
//	NormalOut       = normalize(Normal0);					
//	TexCoordOut     = vec3(TexCoord0, 0.0);				
}
