#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

uniform mat4 VMatrix;
uniform mat4 PMatrix;
const float width = 0.02f;
const float zOffset = 0.02f;

void emit_vertex(vec4 pos)
{
    gl_Position = PMatrix * (pos + vec4(0., 0., zOffset, 0.));
    EmitVertex();
}

void emit_edge(vec4 pos1, vec4 pos2)
{
    vec2 vec = pos2.xy - pos1.xy;
    vec2 ortho_vec = vec2(-vec.y, vec.x);
    vec4 orthogonal = width * vec4(ortho_vec, 0., 0.);
    emit_vertex(pos1 + orthogonal);
    emit_vertex(pos1 - orthogonal);
    emit_vertex(pos2 + orthogonal);
    emit_vertex(pos2 - orthogonal);
    EndPrimitive();
}

void main()
{
    vec4 pos1 = VMatrix * vec4(gl_in[0].gl_Position.xyz, 1.);
    vec4 pos2 = VMatrix * vec4(gl_in[1].gl_Position.xyz, 1.);
    vec4 pos3 = VMatrix * vec4(gl_in[2].gl_Position.xyz, 1.);
    emit_edge(pos1, pos2);
    emit_edge(pos1, pos3);
    emit_edge(pos2, pos3);
}
