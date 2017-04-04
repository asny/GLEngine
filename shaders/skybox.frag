#version 330

uniform samplerCube texture0;
uniform vec3 eyePosition;

in vec3 coords;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 position;

void main() {
    color = texture(texture0, coords);
    position = vec4(eyePosition + normalize(coords) * 100.f, 1.0);
}
