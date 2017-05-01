#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 VMatrix;
uniform mat4 PMatrix;
const float width = 0.1f;

void main()
{
    vec3 pos1 = (VMatrix * gl_in[0].gl_Position.xyzw).xyz;
    vec3 pos2 = (VMatrix * gl_in[1].gl_Position.xyzw).xyz;
    vec3 pos3 = (VMatrix * gl_in[2].gl_Position.xyzw).xyz;
    vec3 orthogonal = cross(normalize(pos2 - pos1), vec3(0., 0., -1.));
    
//    vec3 pos = pos1 + width * orthogonal;
//    gl_Position = PMatrix * vec4(pos, 1.);
//    EmitVertex();
//    
//    pos = pos1 - width * orthogonal;
//    gl_Position = PMatrix * vec4(pos, 1.);
//    EmitVertex();
//    
//    pos = pos2 + width * orthogonal;
//    gl_Position = PMatrix * vec4(pos, 1.);
//    EmitVertex();
//    
//    pos = pos2 - width * orthogonal;
//    gl_Position = PMatrix * vec4(pos, 1.);
//    EmitVertex();
    
    gl_Position = PMatrix * vec4(pos1, 1.);
    EmitVertex();
    gl_Position = PMatrix * vec4(pos2, 1.);
    EmitVertex();
    gl_Position = PMatrix * vec4(pos3, 1.);
    EmitVertex();
}
