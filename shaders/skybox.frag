#version 150

uniform samplerCube texture0;

in vec3 coords;

out vec4 fragColour;

void main() {
    fragColour = texture(texture0, coords);
}