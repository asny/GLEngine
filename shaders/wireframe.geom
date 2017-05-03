#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 VMatrix;
uniform mat4 PMatrix;
const float width = 0.02f;
const float zOffset = 0.02f;

out vec2 barycentric_coords;

void emit_vertex(int index)
{
    vec4 pos = VMatrix * vec4(gl_in[index].gl_Position.xyz, 1.);
    gl_Position = PMatrix * (pos + vec4(0., 0., zOffset, 0.));
    if(index == 0)
        barycentric_coords = vec2(0., 0.);
    if(index == 1)
        barycentric_coords = vec2(1., 0.);
    if(index == 2)
        barycentric_coords = vec2(0., 1.);
        
    EmitVertex();
}

void main()
{
    emit_vertex(0);
    emit_vertex(1);
    emit_vertex(2);
}
