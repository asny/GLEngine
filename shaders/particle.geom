#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 PMatrix;
uniform float radius;

out vec2 posLocal;

void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;
    
    posLocal = vec2(-3., -1.);
    vec3 pos = center + radius * vec3(-3., -1., 0.);
    gl_Position = PMatrix * vec4(pos, 1.);
    EmitVertex();
    
    posLocal = vec2(3., -1.);
    pos = center + radius * vec3(3., -1., 0.);
    gl_Position = PMatrix * vec4(pos, 1.);
    EmitVertex();
    
    posLocal = vec2(0., 2.);
    pos = center + radius * vec3(0., 2., 0.);
    gl_Position = PMatrix * vec4(pos, 1.);
    EmitVertex();
}
