#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 VPMatrix;

out vec2 barycentric_coords;

void emit_vertex(int index)
{
    gl_Position = VPMatrix * vec4(gl_in[index].gl_Position.xyz, 1.);
    EmitVertex();
}

void main()
{
    barycentric_coords = vec2(0., 0.);
    emit_vertex(0);
    barycentric_coords = vec2(1., 0.);
    emit_vertex(1);
    barycentric_coords = vec2(0., 1.);
    emit_vertex(2);
}
