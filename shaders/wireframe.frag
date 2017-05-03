#version 330

uniform vec3 materialColor;

in vec2 barycentric_coords;

layout (location = 0) out vec4 color;

const float oneOverSqrtTwo = 1/sqrt(2);

void main()									
{
    float dist = min(min(barycentric_coords.x, barycentric_coords.y), (1. - barycentric_coords.x - barycentric_coords.y) * oneOverSqrtTwo);
	color = vec4(materialColor, max(1.0 - 100. * dist*dist, 0.));
}
