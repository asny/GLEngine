#version 150

uniform sampler2D texture0;

in vec2 coords;

out vec4 fragColour;

void main() {
    fragColour = texture(texture0, coords);
}