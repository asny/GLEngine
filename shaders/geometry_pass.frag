#version 330
                                                                                       
in vec3 nor;
in vec3 pos;

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 color;
layout (location = 2) out vec4 normal;

void main()									
{											
	position     = vec4(pos, 1.0);
	color      = vec4(1.0, 0.0, 1.0, 1.0);
	normal       = vec4(normalize(nor), 1.0);
}
