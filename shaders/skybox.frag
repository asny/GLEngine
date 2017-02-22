#version 330

uniform samplerCube texture0;

in vec3 coords;

layout (location = 0) out vec4 color;

void main() {
    color = texture(texture0, coords);
}
