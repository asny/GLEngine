#version 150

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 PMatrix;

out vec3 pos;
out vec3 nor;

void main()
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;
    
    nor = normalize(cross(p2 - p1, p3 - p1));
    
    pos = p1;
    gl_Position = PMatrix * vec4(pos, 1.);
    EmitVertex();
    
    pos = p2;
    gl_Position = PMatrix * vec4(pos, 1.);
    EmitVertex();
    
    pos = p3;
    gl_Position = PMatrix * vec4(pos, 1.);
    EmitVertex();
    
    EndPrimitive();
}